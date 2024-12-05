#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "D_Interactable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UD_Interactable : public UInterface
{
	GENERATED_BODY()
};

class DREAM_API ID_Interactable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category="Interaction", meta = (DisplayName = "Interact"))
	void Interact(class AD_Character* CharacterInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category="Interaction", meta = (DisplayName = "Can Interact"))
	bool CanInteract(class AD_Character* CharacterInstigator);
	
};