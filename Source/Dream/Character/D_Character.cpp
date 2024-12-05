#include "D_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Dream/D_CharacterStats.h"
#include "Engine/DataTable.h"
#include "Dream/D_Interactable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Dream/BaseWeaponProjectile/D_BaseWeaponProjectile.h"
#include "Dream/WeaponProjectileComponent/D_WeaponProjectileComponent.h"
#include "Dream/GameInstance/D_GameInstance.h"
#include "Dream/D_CharacterSkins.h"
#include "Net/UnrealNetwork.h"

AD_Character::AD_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>
		(TEXT("NoiseEmitter"));
	NoiseEmitter->NoiseLifetime = 0.01f;

	Weapon = CreateDefaultSubobject<UD_WeaponProjectileComponent>
		(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetRelativeLocation(FVector(120.0f, 70.0f, 0.0f));
	Weapon->SetIsReplicated(true);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.0f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshAsset(TEXT("/Game/KayKit/Characters/rogue"));
	if(SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
}

void AD_Character::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UpdateCharacterStats(1);

	if(IsLocallyControlled())
	{
		if(const auto GameInstanceCast = Cast<UD_GameInstance>(GetWorld()->GetGameInstance());GameInstanceCast != nullptr)
		{
			SetSkinIndex_Server(GameInstanceCast->SkinIndex);
		}
	}
	
}

void AD_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetLocalRole() != ROLE_Authority) return;
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this);

	auto SphereRadius = 50.f;
	auto StartLocation =
		GetActorLocation() + GetActorForwardVector() * 150.f;
	auto EndLocation =
		StartLocation + GetActorForwardVector() * 500.f;
	auto IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		SphereRadius,UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,HitResult,true);
	if(IsHit && HitResult.GetActor()->GetClass()->ImplementsInterface(UD_Interactable::StaticClass()))
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint,SphereRadius,
			12,FColor::Magenta,false,1.f);

		InteractableActor = HitResult.GetActor();
	}
	else
	{
		InteractableActor = nullptr;
	}

	if(GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->
		SprintSpeed)
	{
		auto Noise = 1.f;
		if(GetCharacterStats() && GetCharacterStats()->StealthMultiplier)
		{
			Noise = Noise / GetCharacterStats()->StealthMultiplier;
		}
		NoiseEmitter->MakeNoise(this, Noise, GetActorLocation());
	}
}

void AD_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AD_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AD_Character::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AD_Character::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AD_Character::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AD_Character::SprintEnd);
	}

}

void AD_Character::Move(const struct FInputActionValue& Value)
{
	const auto MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		const auto Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AD_Character::Look(const FInputActionValue& Value)
{
	const auto LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AD_Character::SprintStart(const FInputActionValue& Value)
{
	SprintStart_Server();
}

void AD_Character::SprintEnd(const FInputActionValue& Value)
{
	SprintEnd_Server();
}

void AD_Character::Interact(const FInputActionValue& Value)
{
	Interact_Server();
}

void AD_Character::Interact_Server_Implementation()
{
	if(InteractableActor)
	{
		ID_Interactable::Execute_Interact(InteractableActor, this);
	}
}

void AD_Character::UpdateCharacterStats(int32 CharacterLevel)
{
	auto IsSprinting = false;
	if(GetCharacterStats())
	{
		IsSprinting = GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed;
	}
	
	if(CharacterDataTable)
	{
		TArray<FD_CharacterStats*> CharacterStatsRows;
		CharacterDataTable->GetAllRows<FD_CharacterStats>(TEXT("D_Character"), CharacterStatsRows);
		if(CharacterStatsRows.Num() > 0)
		{
			const auto NewCharacterLevel =
				FMath::Clamp(CharacterLevel, 1, CharacterStatsRows.Num());

			CharacterStats = CharacterStatsRows[NewCharacterLevel - 1];

			GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
			if(IsSprinting)
			{
				SprintStart_Server();
			}
		}
	}
}

void AD_Character::SprintStart_Server_Implementation()
{
	SprintStart_Client();
}

void AD_Character::SprintEnd_Server_Implementation()
{
	SprintEnd_Client();
}

void AD_Character::SprintStart_Client_Implementation()
{
	if(GetCharacterStats())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->SprintSpeed;
	}
}

void AD_Character::SprintEnd_Client_Implementation()
{
	if(GetCharacterStats())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
	}
}

void AD_Character::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AD_Character, SkinIndex);
	
}

void AD_Character::OnRep_SkinChanged(int32 OldValue)
{
	UpdateCharacterSkin();
}

void AD_Character::SetSkinIndex_Server_Implementation(int32 Value)
{
	SkinIndex = Value;
	UpdateCharacterSkin();
}

void AD_Character::UpdateCharacterSkin()
{
	if(CharacterSkinDataTable)
	{
		TArray<FD_CharacterSkins*> CharacterSkinsRows;
		CharacterSkinDataTable->GetAllRows<FD_CharacterSkins>(TEXT("D_Character"), CharacterSkinsRows);

		if(CharacterSkinsRows.Num() > 0)
		{
			const auto Index = FMath::Clamp(SkinIndex, 0 , CharacterSkinsRows.Num() - 1);
			CharacterSkin = CharacterSkinsRows[Index];

			GetMesh()->SetMaterial(4, CharacterSkin->Material4);
			GetMesh()->SetMaterial(0, CharacterSkin->Material0);
			GetMesh()->SetMaterial(1, CharacterSkin->Material1);
			GetMesh()->SetMaterial(2, CharacterSkin->Material2);
		}
		
	}
}

