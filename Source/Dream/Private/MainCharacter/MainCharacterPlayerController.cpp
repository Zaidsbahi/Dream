#include "MainCharacter/MainCharacterPlayerController.h"
#include "MainCharacter/MainCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AMainCharacterPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	
	// Store a Reference to Main Character
	MainCharacter = Cast<AMainCharacter>(aPawn);
	checkf(MainCharacter, TEXT("AMainCharacter Derived Classes should only possess AMainCharacter derived pawns"));
	

	// Reference to the EnhancedInputComponent
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get reference to EnhancedInputComponent"));

	// Get the local Player Subsystem
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("Unable to get reference to InputSubsystem"));

	// Clearing And Adding MappingContext
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);

	// Binding the input Actions
	// Only Attempt to Bind if a Valid value was provided
	
	if(ActionMove)
		{EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &AMainCharacterPlayerController::HandleMove);}
	
	if(ActionLook)
	{EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this,&AMainCharacterPlayerController::HandleLook);}

	if (ActionJump)
	{EnhancedInputComponent->BindAction(ActionJump,ETriggerEvent::Triggered,this, &AMainCharacterPlayerController::HandleJump);}
}

void AMainCharacterPlayerController::HandleMove(const FInputActionValue& Value)
{
	// Value is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// Add Movement to the MainCharacter
	if(MainCharacter)
	{
		MainCharacter->AddMovementInput(MainCharacter->GetActorForwardVector(),MovementVector.Y);
		MainCharacter->AddMovementInput(MainCharacter->GetActorRightVector(),MovementVector.X);
	}
}

void AMainCharacterPlayerController::HandleLook(const FInputActionValue& Value)
{
	// Value is Vector2D
	const FVector2D LookDirection = Value.Get<FVector2D>();

	AddYawInput(LookDirection.X);
	AddPitchInput(LookDirection.Y);
}

void AMainCharacterPlayerController::HandleJump()
{
	// Input is 'Digital' (Value is not used here)

	// Make the Main Character Jump
	if(MainCharacter)
	{
		MainCharacter->Jump();
	}
}

void AMainCharacterPlayerController::OnUnPossess()
{
	// Unbinding Things Here
	EnhancedInputComponent->ClearActionBindings();
	
	// call the parent method
	Super::OnUnPossess();
}