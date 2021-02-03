// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APlantItemBase::APlantItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MyMesh->SetupAttachment(GetRootComponent());

	CollisionVolume->SetSphereRadius(90.f);
}

// Called when the game starts or when spawned
void APlantItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &APlantItemBase::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &APlantItemBase::OnOverlapEnd);
}

// Called every frame
void APlantItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Call from inherited class by use of "Super::OnOverlapBegin(...)" (override)
void APlantItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Destroy the pickup actor
	//Destroy();
}

void APlantItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}