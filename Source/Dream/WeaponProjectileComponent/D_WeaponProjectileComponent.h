#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "D_WeaponProjectileComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAM_API UD_WeaponProjectileComponent : public USceneComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile", meta=(AllowPrivateAccess=true))
	TSubclassOf<class AD_BaseWeaponProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Input", meta=(AllowPrivateAccess=true))
	class UInputMappingContext* WeaponMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	class UInputAction* ThrowAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile",
		meta=(AllowPrivateAccess=true))
	UAnimMontage* ThrowAnimation;

protected:

	virtual void BeginPlay() override;

	void Throw();

	UFUNCTION(Server, Reliable)
	void Throw_Sever();

	UFUNCTION(NetMulticast, Unreliable)
	void Throw_Client();

public:	

	UD_WeaponProjectileComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SetProjectileClass(TSubclassOf<class AD_BaseWeaponProjectile> NewProjectileClass);
};
