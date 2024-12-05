#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_MinionSpawner.generated.h"

UCLASS()
class DREAM_API AD_MinionSpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Spawn System", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> SpawnArea;

	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
	
public:	

	AD_MinionSpawner();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void Spawn();

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn System")
	TArray<TSubclassOf<class AD_Minion>> SpawnableMinions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn System")
	float SpawnDelay = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn System")
	int32 NumMinionsAtStart = 5;
	
};
