#include "D_MainMenuPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Dream/D_CharacterSkins.h"
#include "Dream/GameInstance/D_GameInstance.h"


AD_MainMenuPawn::AD_MainMenuPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(450.0f, 90.0f, 160.0f));
	Camera->SetRelativeRotation(FRotator(-10.0f, 180.0f, 0.0f));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeLocation(FVector(0.f, -30.f, 90.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshAsset(TEXT("/Game/KayKit/Characters/rogue.rogue"));
	if(SkeletalMeshAsset.Succeeded())
	{
		Mesh->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
}

void AD_MainMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	if(IsLocallyControlled())
	{
		RandomizeCharacterSkins();
	}
	
}

void AD_MainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AD_MainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AD_MainMenuPawn::RandomizeCharacterSkins()
{
	if(CharacterSkinDataTable)
	{
		TArray<FD_CharacterSkins*> CharacterSkinRows;
		CharacterSkinDataTable->GetAllRows<FD_CharacterSkins>
		(TEXT("D_Character"), CharacterSkinRows);

		if(CharacterSkinRows.Num() > 0)
		{
			const auto NewIndex =
				FMath::RandRange(0, CharacterSkinRows.Num() - 1);

			CharacterSkin = CharacterSkinRows[NewIndex];

			Mesh->SetMaterial(4, CharacterSkinRows[NewIndex]->Material4);
			Mesh->SetMaterial(0, CharacterSkinRows[NewIndex]->Material0);
			Mesh->SetMaterial(1, CharacterSkinRows[NewIndex]->Material1);
			Mesh->SetMaterial(2, CharacterSkinRows[NewIndex]->Material2);

			if(const auto GameInstance = Cast<UD_GameInstance>(GetGameInstance()))
			{
				GameInstance->SkinIndex = NewIndex;
			}
		}
	}
}