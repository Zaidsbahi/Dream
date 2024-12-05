#include "D_PlayerState.h"
#include "Dream/D_CharacterStats.h"
#include "Dream/Character/D_Character.h"
#include "Net/UnrealNetwork.h"


void AD_PlayerState::AddXp(int32 Value)
{
	Xp += Value;
	OnXpChanged.Broadcast(Xp);

	GEngine->AddOnScreenDebugMessage(0, 5.f ,
		FColor::Yellow, FString::Printf(TEXT("Total Xp: %d"), Value));

	if(const auto Character = Cast<AD_Character>(GetPawn()))
	{
		if (Character->GetCharacterStats()->NextLevelUp < Xp)
		{
			GEngine->AddOnScreenDebugMessage(0, 5.f ,FColor::Red, TEXT("Level Up!"));
			CharacterLevel++;
			Character->UpdateCharacterStats(CharacterLevel);
			OnCharacterLevelUp.Broadcast(CharacterLevel);
		}
	}
}

void AD_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AD_PlayerState, Xp, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AD_PlayerState, CharacterLevel, COND_OwnerOnly);
}

void AD_PlayerState::OnRep_Xp(int32 OldValue) const
{
	OnXpChanged.Broadcast(Xp);
}

void AD_PlayerState::OnRep_CharacterLevelUp(int32 OldValue) const
{
	OnCharacterLevelUp.Broadcast(CharacterLevel);
}
