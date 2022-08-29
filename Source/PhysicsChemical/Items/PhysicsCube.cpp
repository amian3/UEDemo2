// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsCube.h"
#include "../BallPawn/BallPawn.h"
#include "BuoyancyComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APhysicsCube::APhysicsCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));

	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	Buoyancy->AddCustomPontoon(50.0f, FVector(0.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void APhysicsCube::BeginPlay()
{
	Super::BeginPlay();

	Cube->SetMaterial(0, MaterialMap[CubeStatus]);

}

// Called every frame
void APhysicsCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsMoveable)
	{
		if (GetWorld() && GetWorld()->GetFirstPlayerController())
		{
			FHitResult HitResult;
			GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel((ETraceTypeQuery)ECollisionChannel::ECC_Visibility, false, HitResult);
			if (HitResult.GetActor() && HitResult.GetActor() != this)
			{
				SetActorLocation(FVector(HitResult.Location.X, HitResult.Location.Y, GetActorLocation().Z));
			}
		}
	}
}

UStaticMeshComponent* APhysicsCube::GetStaticMesh()
{
	return Cube;
}

void APhysicsCube::OnClickCube()
{
	IsMoveable = true;
	Cube->SetSimulatePhysics(false);

	//SetActorEnableCollision(false);
}

void APhysicsCube::OnReleasedCube()
{
	IsMoveable = false;
	Cube->SetSimulatePhysics(true);

	//SetActorEnableCollision(true);
}

void APhysicsCube::SetStatus(CStatus Target)
{
	CubeStatus = Target;
	Cube->SetMaterial(0, MaterialMap[Target]);
}