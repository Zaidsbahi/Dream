#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "D_MainMenuPawn.generated.h"

UCLASS()
class DREAM_API AD_MainMenuPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Arrow", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UArrowComponent> Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Data", meta=(AllowPrivateAccess = "true"))
	class UDataTable* CharacterSkinDataTable;

	struct FD_CharacterSkins* CharacterSkin;
	
protected:

	virtual void BeginPlay() override;

	FORCEINLINE UArrowComponent* GetArrow() { return Arrow; }
	FORCEINLINE UCameraComponent* GetCamera() { return Camera; }
	FORCEINLINE USkeletalMeshComponent* GetMesh() { return Mesh; }

	void RandomizeCharacterSkins();
	
public:
	
	AD_MainMenuPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE FD_CharacterSkins* GetCharacterSkin() const { return CharacterSkin; }
	
};
