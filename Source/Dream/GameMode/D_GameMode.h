#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "D_GameMode.generated.h"

UCLASS()
class DREAM_API AD_GameMode : public AGameMode
{
	GENERATED_BODY()
	AD_GameMode();

public:

	UFUNCTION(BlueprintCallable, Category = "Minions")
	void AlertMinions(class AActor* AlertInstigator, const FVector&
		Location, float Radius);
	
};
