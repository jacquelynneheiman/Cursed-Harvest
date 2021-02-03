// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickupItemBase::APickupItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	CollisionVolume->SetSphereRadius(90.f);
}

// Called when the game starts or when spawned
void APickupItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &APickupItemBase::OnOverlapBegin);
}

// Called every frame
void APickupItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Call from inherited class by use of "Super::OnOverlapBegin(...)" (override)
void APickupItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Destroy the pickup actor
	Destroy();
}
