#include "D_BaseWeaponProjectile.h"
#include "Dream/Character/D_Character.h"
#include "Dream/D_CharacterStats.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

AD_BaseWeaponProjectile::AD_BaseWeaponProjectile()
{

	PrimaryActorTick.bCanEverTick = true;

	SphereCollision =
		CreateDefaultSubobject<USphereComponent>(FName("Collision"));

	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(10.0f);
	SphereCollision->BodyInstance.SetCollisionProfileName("BlockAll");
	SphereCollision->OnComponentHit.AddDynamic(this, &AD_BaseWeaponProjectile::OnHit);

	RootComponent = SphereCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetRelativeLocation(FVector(-40.f, 0.f, 0.f));
	Mesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh
	(TEXT("/Game/KayKit/DungeonElements/dagger_common.dagger_common"));
	if(StaticMesh.Succeeded())
	{
		GetMesh()->SetStaticMesh(StaticMesh.Object);
	}
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->UpdatedComponent = SphereCollision;
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->InitialSpeed = 3000;
	ProjectileMovement->MaxSpeed = 3000;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	
	bReplicates = true;
}

void AD_BaseWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AD_BaseWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	auto ComputedDamage = Damage;
	if(const auto Character = Cast<AD_Character>(GetInstigator()))
	{
		ComputedDamage *= Character->GetCharacterStats()->DamageMultiplier;
	}
	if(OtherActor && OtherActor != this)
	{
		const FDamageEvent Event (UDamageType::StaticClass());
		OtherActor->TakeDamage(ComputedDamage,Event,
			GetInstigatorController(),this);
	}
	Destroy();
}

void AD_BaseWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

