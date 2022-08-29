// Fill out your copyright notice in the Description page of Project Settings.


#include "PushBox.h"
#include "Components/StaticMeshComponent.h"
#include "../BallPawn/BallPawn.h"
// Sets default values
APushBox::APushBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
}

// Called when the game starts or when spawned
void APushBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APushBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InRefresh)
	{
		RefreshTimer -= DeltaTime;
		if (RefreshTimer <= 0.0f)
		{
			InRefresh = false;
		}
	}

}

void APushBox::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, 
	class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, 
	FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(Other);
		if (PlayerBall)
		{
			if (!InRefresh)
			{
				FVector Direction = HitNormal;
				Direction.Z = 0.0f;
				if (Direction.X >= 0.0f && Direction.X >= fabs(Direction.Y))
				{
					Direction.Y = 0.0f;
					Direction.X = 1.0f;
				}
				else if (Direction.X < 0.0f && -Direction.X >= fabs(Direction.Y))
				{
					Direction.X = -1.0f;
					Direction.Y = 0.0f;
				}
				else if (Direction.Y >= 0.0f && Direction.Y >= fabs(Direction.X))
				{
					Direction.X = 0.0f;
					Direction.Y = 1.0f;
				}
				else if (Direction.Y < 0.0f && -Direction.Y >= fabs(Direction.X))
				{
					Direction.Y = -1.0f;
					Direction.X = 0.0f;
				}

				Box->AddImpulse(PushImpluse * Direction);
				RefreshTimer = RefreshBound;
			}

		}
	}
}
