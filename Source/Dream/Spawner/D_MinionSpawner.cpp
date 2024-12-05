#include "D_MinionSpawner.h"
#include "Dream/Minion/D_Minion.h"
#include "Components/BoxComponent.h"

AD_MinionSpawner::AD_MinionSpawner()
{

	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>("Spawn Area");
	SpawnArea->SetupAttachment(RootComponent);
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));
	
}

void AD_MinionSpawner::BeginPlay()
{
	Super::BeginPlay();

	if(SpawnableMinions.IsEmpty()) return;
	if(GetLocalRole() !=ROLE_Authority) return;

	for (int32 i = 0; i < NumMinionsAtStart; i++)
	{
		Spawn();
	}

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this,
		&AD_MinionSpawner::Spawn, SpawnDelay,true,SpawnDelay);
	
}

void AD_MinionSpawner::Spawn()
{

	FActorSpawnParameters SpawnParams;
	
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	auto Minion =
		SpawnableMinions[FMath::RandRange(0,SpawnableMinions.Num() - 1)];

	const auto Rotation =
		FRotator(0.0f, FMath::RandRange(0.0f, 360.0f),0.0f);

	const auto Location =
		SpawnArea->GetComponentLocation() +
			FVector(
				FMath::RandRange(-SpawnArea->GetScaledBoxExtent().X,SpawnArea->GetScaledBoxExtent().X),
				FMath::RandRange(-SpawnArea->GetScaledBoxExtent().Y,SpawnArea->GetScaledBoxExtent().Y),
				0.0f);

	GetWorld()->SpawnActor<AD_Minion>(Minion, Location, Rotation,
		SpawnParams);
	
}

void AD_MinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
