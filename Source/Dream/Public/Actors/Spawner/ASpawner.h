// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASpawner.generated.h"

UCLASS()
class DREAM_API AASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Fixed Array of Spawn Points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning");
	TArray<FVector> SpawnPoints;

	// The mesh im going to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "Spawning")
	TArray<TSubclassOf<AActor>> MeshesToSpawn;

	// Map for associating meshes with color
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Spawning")
	TMap<TSubclassOf<AActor>, FLinearColor> MeshColorMap;
	
	// Function to spawn mesh at all the spawn points
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnMeshes();

private:
	const int32 MaxSpawnPoints = 5;

	//Function to apply the color on the mesh
	void ApplyColor(AActor* SpawnedMesh, FLinearColor Color);
};
