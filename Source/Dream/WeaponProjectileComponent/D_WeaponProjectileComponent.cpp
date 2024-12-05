#include "D_WeaponProjectileComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Dream/BaseWeaponProjectile/D_BaseWeaponProjectile.h"
#include "Dream/Character/D_Character.h"

UD_WeaponProjectileComponent::UD_WeaponProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ProjectileClass = AD_BaseWeaponProjectile::StaticClass();
}

void UD_WeaponProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;

	if(const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(WeaponMappingContext, 1);
		}

		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(ThrowAction,
				ETriggerEvent::Triggered, this, &UD_WeaponProjectileComponent::Throw);
		}
	}
	
}

void UD_WeaponProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UD_WeaponProjectileComponent::SetProjectileClass
(TSubclassOf<class AD_BaseWeaponProjectile> NewProjectileClass)
{
	ProjectileClass = NewProjectileClass;
}

void UD_WeaponProjectileComponent::Throw()
{
	Throw_Sever();
}

void UD_WeaponProjectileComponent::Throw_Sever_Implementation()
{
	if (ProjectileClass)
	{
		Throw_Client();
		FTimerHandle TimerHandle;
	
		 GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		 {
		 const auto Character = Cast<AD_Character>(GetOwner());
		 const auto ProjectileSpawnLocation = GetComponentLocation();
		 const auto ProjectileSpawnRotation = GetComponentRotation();
		 auto ProjectileSpawnParams = FActorSpawnParameters();
		 ProjectileSpawnParams.Owner = GetOwner();
		 ProjectileSpawnParams.Instigator = Character;
		 GetWorld()->SpawnActor<AD_BaseWeaponProjectile>(ProjectileClass,
		ProjectileSpawnLocation, ProjectileSpawnRotation,
		ProjectileSpawnParams);
		 }, .4f, false);
	}
}

void UD_WeaponProjectileComponent::Throw_Client_Implementation()
{
	const auto Character = Cast<AD_Character>(GetOwner());
	if(ThrowAnimation != nullptr)
	{
		if(const auto AnimInstance =
			Character->GetMesh()->GetAnimInstance(); AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(ThrowAnimation, 1.f);
		}
	}
}