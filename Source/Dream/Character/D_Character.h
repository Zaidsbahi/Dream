#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "D_Character.generated.h"

UCLASS()
class DREAM_API AD_Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= Camera,meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= Camera,meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Data" , meta=(AllowPrivateAccess = "true"))
	class UDataTable* CharacterDataTable;

	struct FD_CharacterStats* CharacterStats;

	UPROPERTY()
	AActor* InteractableActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Stealth"
		, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPawnNoiseEmitterComponent> NoiseEmitter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon",
		meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UD_WeaponProjectileComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Data", meta=(AllowPrivateAccess = "true"))
	UDataTable* CharacterSkinDataTable;

	struct FD_CharacterSkins* CharacterSkin;
	
protected:

	virtual void BeginPlay() override;

	void Move(const struct FInputActionValue& Value);	
	void Look(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprintEnd(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

	UFUNCTION(Server,Reliable)
	void Interact_Server();

	UFUNCTION(Server, Reliable)
	void SprintStart_Server();
	
	UFUNCTION(Server, Reliable)
	void SprintEnd_Server();

	UFUNCTION(NetMulticast,Reliable)
	void SprintStart_Client();

	UFUNCTION(NetMulticast,Reliable)
	void SprintEnd_Client();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_SkinChanged", Category= "Skin")
	int32 SkinIndex = 0;

	UFUNCTION()
	void OnRep_SkinChanged(int32 OldValue);

	UFUNCTION(Server, Reliable)
	void SetSkinIndex_Server(int32 Value);

	UFUNCTION()
	void UpdateCharacterSkin();
	
public:

	AD_Character();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return FollowCamera; }

	void UpdateCharacterStats(int32 CharacterLevel);

	FORCEINLINE FD_CharacterStats* GetCharacterStats() const { return CharacterStats; }

	FORCEINLINE UD_WeaponProjectileComponent* GetWeapon() const {return Weapon;}


	FORCEINLINE FD_CharacterSkins* GetCharacterSkins() const {return CharacterSkin;}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};
