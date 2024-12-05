#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "D_GameInstance.generated.h"

UCLASS()
class DREAM_API UD_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	int32 SkinIndex;
	
};
