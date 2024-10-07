// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Spawner/ASpawner.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AASpawner::AASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	//Initializing Array with fixed Size
	SpawnPoints.SetNum(MaxSpawnPoints);
	SpawnPoints[0] = FVector(0.0f, 0.0f, 100.0f);
	SpawnPoints[1] = FVector(200.0f, 100.0f, 100.0f);
	SpawnPoints[2] = FVector(-200.0f, -100.0f, 100.0f);
	SpawnPoints[3] = FVector(300.0f, 200.0f, 100.0f);
	SpawnPoints[4] = FVector(-300.0f, -200.0f, 100.0f);
}

// Called when the game starts or when spawned
void AASpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnMeshes();
}

// Called every frame
void AASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AASpawner::SpawnMeshes()
{
	// Check if mesh to spawn is set
	if(MeshesToSpawn.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, TEXT("No Meshes To Spawn!"));
		return;
	}

	for (const FVector& SpawnPoint : SpawnPoints)
	{
		// Picks a random Mesh from the list
		int32 RandomMeshIndex = FMath::RandRange(0,MeshesToSpawn.Num() - 1);
		TSubclassOf<AActor> MeshToSpawn = MeshesToSpawn[RandomMeshIndex];

		//Spawn the mesh at the location
		AActor* SpawnedMesh = GetWorld()->SpawnActor<AActor>(MeshToSpawn,SpawnPoint,FRotator::ZeroRotator);

		if(SpawnedMesh)
		{
			//Checking if the mesh has an associated color in the map
			FLinearColor* Color = MeshColorMap.Find(MeshToSpawn);
			if(Color)
			{
				//Apply the color to the mesh
				ApplyColor(SpawnedMesh, *Color);
				GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,TEXT("Color Applied to Mesh!"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No color found for this mesh in the map."));
			}
		}
		
	}	
}

void AASpawner::ApplyColor(AActor* SpawnedMesh, FLinearColor Color)
{
	// Get the StaticMeshComponent from the spawned actor
	UStaticMeshComponent* MeshComponent = SpawnedMesh->FindComponentByClass<UStaticMeshComponent>();

	if(MeshComponent)
	{
		//Create a Dynamic material instance and apply the color
		UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
		if(DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", Color); // The material need a base color
		}
	}
}

