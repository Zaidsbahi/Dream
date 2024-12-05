#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_BaseWeaponProjectile.generated.h"

UCLASS()
class DREAM_API AD_BaseWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AD_BaseWeaponProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "Components",
		meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "Components",
		meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "Components",
		meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "Weapon",
		meta=(AllowPrivateAccess = "true"))
	float Damage = 1.f;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,FVector NormalImpulse,
		const FHitResult& Hit);

public:	

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
};
