#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "D_CharacterSkins.generated.h"

USTRUCT(BlueprintType)
struct DREAM_API FD_CharacterSkins : public
FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BluePrintReadWrite, EditAnywhere)
	UMaterialInterface *Material4;
	
	UPROPERTY(BluePrintReadWrite, EditAnywhere)
	UMaterialInterface *Material0;
	
	UPROPERTY(BluePrintReadWrite, EditAnywhere)
	UMaterialInterface *Material1;
	
	UPROPERTY(BluePrintReadWrite, EditAnywhere)
	UMaterialInterface *Material2;
};
