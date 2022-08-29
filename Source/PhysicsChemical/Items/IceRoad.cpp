// Fill out your copyright notice in the Description page of Project Settings.


#include "IceRoad.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AIceRoad::AIceRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Plane.Plane"));//
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RoadMesh(TEXT("/Game/Items/meshs/SM_Plane1000_512.SM_Plane1000_512"));

	Road = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Road"));
	Road->SetStaticMesh(RoadMesh.Object);
	RootComponent = Road;
	Road->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	//Road->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AIceRoad::BeginPlay()
{
	Super::BeginPlay();
	DestroyTimer = DestroyBound;
	
}

// Called every frame
void AIceRoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DestroyTimer -= DeltaTime;
	if (DestroyTimer < 0.0f)
	{
		Destroy();
	}

}

