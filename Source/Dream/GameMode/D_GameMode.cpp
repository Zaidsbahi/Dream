#include "D_GameMode.h"
#include "Dream/GameState/D_GameState.h"
#include "Dream/PlayerController/D_PlayerController.h"
#include "Dream/PlayerState/D_PlayerState.h"
#include "Dream/Character/D_Character.h"
#include "Dream/Minion/D_Minion.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Dream/D_CustomLog.h"

AD_GameMode::AD_GameMode()
{
	GameStateClass = AD_GameState::StaticClass();
	PlayerStateClass = AD_PlayerState::StaticClass();
	PlayerControllerClass = AD_PlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn>PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Character"));
	if(PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AD_GameMode::AlertMinions(class AActor* AlertInstigator,
	const FVector& Location, float Radius)
{
	UE_LOG(LogUnrealDream, Display, TEXT("Alerting Minions"));
	
	TArray<AActor*> Minions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		AD_Minion::StaticClass(), Minions);

	for (const auto Minion : Minions)
	{
		if(AlertInstigator == Minion) continue;
		if(const auto Distance = FVector::Distance(AlertInstigator->
			GetActorLocation(), Minion->GetActorLocation()); Distance < Radius )
		{
			if(const auto MinionCharacter = Cast<AD_Minion>(Minion))
			{
				MinionCharacter->GoToLocation(Location);
			}
		}
	}
}





