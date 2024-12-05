#include "D_BasePickup.h"
#include "Dream/Character/D_Character.h"
#include "Components/SphereComponent.h"

AD_BasePickup::AD_BasePickup()
{

	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = SphereCollision;
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(200.0f);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
	
}

void AD_BasePickup::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AD_BasePickup::AD_BasePickup::OnBeginOverlap);
}

void AD_BasePickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(const auto Character = Cast<AD_Character>(OtherActor))
	{
		Pickup(Character);
	}
}


void AD_BasePickup::Pickup_Implementation( AD_Character* OwningCharacter)
{
	SetOwner(OwningCharacter);
}

void AD_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

