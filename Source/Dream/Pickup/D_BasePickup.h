#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_BasePickup.generated.h"

UCLASS()
class DREAM_API AD_BasePickup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

public:	

	AD_BasePickup();

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BluePrintCallable, BlueprintNativeEvent, Category = "Pickup",meta=(DisplayName="Pickup"))
	void Pickup(class AD_Character* OwningCharacter);
	
public:	

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return Mesh; }

};
