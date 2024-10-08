#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterPlayerController.generated.h"

// Forward Declarations
struct FInputActionValue;
class AMainCharacter;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputComponent;

UCLASS(Abstract)
class DREAM_API AMainCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	// Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionMove = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionLook = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionJump = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

protected:

	// Handlers
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();

	// Overriding Members
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	
private:
	
	// Reference to the inputComponent cast to an EnhancedInputComponent
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;

	// Reference To Main Character
	UPROPERTY()
	TObjectPtr<AMainCharacter> MainCharacter = nullptr;
};
