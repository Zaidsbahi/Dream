#include "D_Minion.h"
#include "NavigationSystem.h"
#include "Dream/Character/D_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBluePrintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Dream/GameMode/D_GameMode.h"
#include "Dream/Pickup/D_BasePickup.h"

AD_Minion::AD_Minion()
{

	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	PawnSense=
		CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	PawnSense->SensingInterval = .8f;
	PawnSense->SetPeripheralVisionAngle(45.f);
	PawnSense->SightRadius = 1500.f;
	PawnSense->HearingThreshold = 400.f;
	PawnSense->LOSHearingThreshold = 800.f;

	Collision=
		CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(100);
	Collision->SetupAttachment(RootComponent);

	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	SkeletalMeshAsset(TEXT("/Game/KayKit/Skeletons/skeleton_minion.skeleton_minion"));
	if(SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	static ConstructorHelpers::FClassFinder<AD_BasePickup>
	SpawnedPickupAsset(TEXT("/Game/Blueprints/BP_GoldCoinPickup.BP_GoldCoinPickup"));
	if (SpawnedPickupAsset.Succeeded())
	{
		SpawnedPickup = SpawnedPickupAsset.Class;
	}
	
}

void AD_Minion::BeginPlay()
{
	Super::BeginPlay();

	SetNextPatrolLocation();
	
}

void AD_Minion::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f,
		FColor::Green, TEXT("Noise detected!"));

	GoToLocation(Location);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(),
		PatrolLocation);
}

void AD_Minion::OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	Health -= Damage;
	if(Health > 0) return;

	if(SpawnedPickup)
	{
		GetWorld()->SpawnActor<AD_BasePickup>(SpawnedPickup, GetActorLocation(), GetActorRotation());
	}
	Destroy();
}

void AD_Minion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetLocalRole() != ROLE_Authority) return;

	if(GetMovementComponent()->GetMaxSpeed() == ChaseSpeed) return;

	if((GetActorLocation() - PatrolLocation).Size() < 500.f)
	{
		SetNextPatrolLocation();
	}

}

void AD_Minion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AD_Minion::SetNextPatrolLocation()
{
	if(GetLocalRole() != ROLE_Authority) return;

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

	const auto LocationFound =
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(
			this,GetActorLocation(),PatrolLocation,PatrolRadius);

	if(LocationFound)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation
		(GetController(), PatrolLocation);
	}
}

void AD_Minion::Chase(APawn* Pawn)
{
	if(GetLocalRole() != ROLE_Authority) return;

	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	UAIBlueprintHelperLibrary::SimpleMoveToActor
	(GetController(), Pawn);

	DrawDebugSphere(GetWorld(),
		Pawn->GetActorLocation(),
		25.f, 12, FColor::Red,
		true, 10.f, 0, 2.f);

	if(const auto GameMode = Cast<AD_GameMode>
		(GetWorld()->GetAuthGameMode()))
	{
		GameMode->AlertMinions(this, Pawn->GetActorLocation(), AlertRadius);
	}
}

void AD_Minion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(GetLocalRole() != ROLE_Authority) return;

	OnActorBeginOverlap.AddDynamic
	(this, &AD_Minion::OnBeginOverlap);
	
	GetPawn_Sense()->OnSeePawn.AddDynamic
	(this, &AD_Minion::OnPawnDetected);

	GetPawn_Sense()->OnHearNoise.AddDynamic
	(this, &AD_Minion::OnHearNoise);

	OnTakeAnyDamage.AddDynamic
	(this, &AD_Minion::OnDamage);

}

void AD_Minion::OnPawnDetected(APawn* Pawn)
{
	if(!Pawn->IsA<AD_Character>()) return;

	GEngine->AddOnScreenDebugMessage
	(-1, 5.f, FColor::Red, "Character Detected");

	if(GetCharacterMovement()->MaxWalkSpeed != ChaseSpeed)
	{
		Chase(Pawn);
	}
	
}

void AD_Minion::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!OtherActor->IsA<AD_Character>()) return;

	GEngine->AddOnScreenDebugMessage
	(-1, 5.f, FColor::Red, "Character Captured");
}

void AD_Minion::GoToLocation(const FVector& Location)
{
	PatrolLocation = Location;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(),
		PatrolLocation);
}

