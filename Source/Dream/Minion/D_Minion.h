#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "D_Minion.generated.h"

UCLASS()
class DREAM_API AD_Minion : public ACharacter
{
	GENERATED_BODY()

public:

	AD_Minion();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Minion Perception"
		,meta = (AllowPrivateAccess=true))
	TObjectPtr<class UPawnSensingComponent> PawnSense;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Minion",
		meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Collision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup",
	meta=(AllowPrivateAccess = "true"))
	TSubclassOf<class AD_BasePickup> SpawnedPickup;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Minion AI",
		meta=(AllowPrivateAccess = "true"))
	float AlertRadius = 6000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health",
		meta=(AllowPrivateAccess = "true"))
	float Health = 5.f;

	UPROPERTY()
	FVector PatrolLocation;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float
		Volume);

	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const UDamageType*
		DamageType,AController* InstigatedBy, AActor* DamageCauser);

public:	

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Minion AI")
	float PatrolSpeed = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Minion AI")
	float ChaseSpeed = 350.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Minion AI")
	float PatrolRadius = 50000.0f;

	UFUNCTION(BlueprintCallable, Category="Minion AI")
	void SetNextPatrolLocation();

	UFUNCTION(BlueprintCallable, Category="Minion AI")
	void Chase(APawn* Pawn);

	FORCEINLINE UPawnSensingComponent* GetPawn_Sense()
	const { return PawnSense; }

	FORCEINLINE USphereComponent* GetCollision()
	const {return Collision;}

	UFUNCTION()
	void OnPawnDetected(APawn* Pawn);

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION(BlueprintCallable, Category="Minion AI")
	void GoToLocation(const FVector& Location);

	
};
