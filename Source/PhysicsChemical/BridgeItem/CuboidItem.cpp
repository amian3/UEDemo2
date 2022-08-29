// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboidItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
// Sets default values
ACuboidItem::ACuboidItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = Cube;

	CubeX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeX"));
	CubeY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeY"));
	CubeZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeZ"));

	
	CubeX->AttachTo(RootComponent);
	CubeY->AttachTo(RootComponent);
	CubeZ->AttachTo(RootComponent);
	CubeX->SetVisibility(false);
	CubeY->SetVisibility(false);
	CubeZ->SetVisibility(false);

	ArrowX = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowX"));
	ArrowX->AttachTo(RootComponent);
	ArrowX->SetArrowColor(FLinearColor(255.0f, 0.0f, 0.0f, 255.0f));
	ArrowY = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowY"));
	ArrowY->AttachTo(RootComponent);
	ArrowY->SetArrowColor(FLinearColor(0.0f, 255.0f, 0.0f, 255.0f));
	//ArrowY->SetRelativeRotation(FRotator(-90.0f, 0.0f, 90.0f));
	ArrowZ = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowZ"));
	ArrowZ->AttachTo(RootComponent);
	ArrowZ->SetArrowColor(FLinearColor(0.0f, 0.0f, 255.0f, 255.0f));
	//ArrowZ->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
}

// Called when the game starts or when spawned
void ACuboidItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuboidItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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

void ACuboidItem::OnClickCuboidItem()
{
	if (ArrowIsVisible)
	{
		ArrowIsVisible = false;
		CubeX->SetVisibility(false);
		CubeY->SetVisibility(false);
		CubeZ->SetVisibility(false);
		ArrowX->SetHiddenInGame(true);
		ArrowY->SetHiddenInGame(true);
		ArrowZ->SetHiddenInGame(true);

	}
	else
	{

		ArrowIsVisible = true;
		CubeX->SetVisibility(true);
		CubeY->SetVisibility(true);
		CubeZ->SetVisibility(true);
		ArrowX->SetHiddenInGame(false);
		ArrowY->SetHiddenInGame(false);
		ArrowZ->SetHiddenInGame(false);
	}

}

void ACuboidItem::MoveByAxis(uint8 Axis)
{
	MoveAxis = Axis;
}

void ACuboidItem::OnReleasedCuboid()
{
	MoveAxis = 0;
}