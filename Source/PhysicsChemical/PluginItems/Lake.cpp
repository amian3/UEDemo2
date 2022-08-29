// Fill out your copyright notice in the Description page of Project Settings.


#include "Lake.h"
#include "Materials/MaterialInterface.h"
#include "../BallPawn/BallPawn.h"
#include "../Items/PhysicsCube.h"
#include "WaterSplineComponent.h"
#include "Components/TimelineComponent.h"
__pragma (optimize("", off))

ALake::ALake()
{
	PrimaryActorTick.bCanEverTick = true;
	SplineComp->AddSplineLocalPoint(FVector(500.0f, 500.0f, 0.0f));
}

void ALake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ColorChangeTimer != 0)
	{
		ColorChangeTimer -= 1;
		FLinearColor CurrentColor = ((float)ColorChangeTimer / (float)ColorChangeBound) * ColorMap[LastStatus] + 
			((float)(ColorChangeBound - ColorChangeTimer) / (float)ColorChangeBound) * ColorMap[CurrentStatus];
		GetWaterMaterialInstance()->SetVectorParameterValue(TEXT("Absorption"), CurrentColor);
	}
}

void ALake::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ALake::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ALake::OnEndOverlap);

	GetWaterMaterialInstance()->SetVectorParameterValue(TEXT("Absorption"), ColorMap[CurrentStatus]);
}

void ALake::OnBeginOverlap(AActor* PrimitiveActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall)
		{
			if ((CurrentStatus == FluidStatus::CuSO4 && PlayerBall->BallStatus == ProperyStatus::Iron)
				|| (CurrentStatus == FluidStatus::FeCl3 && PlayerBall->BallStatus == ProperyStatus::Zinc)
				|| (CurrentStatus == FluidStatus::ZnCl2 && PlayerBall->BallStatus == ProperyStatus::Copper))//update to water
			{
				LastStatus = CurrentStatus;
				CurrentStatus = FluidStatus::Water;
				ColorChangeTimer = ColorChangeBound;
				PlayerBall->ForceSetIce();
			}
			else if(CurrentStatus != FluidStatus::Water || PlayerBall->BallStatus != ProperyStatus::Ice)
			{
				PlayerBall->GetVanishTimeline()->SetPlaybackPosition(0.0f, false);
				PlayerBall->GetVanishTimeline()->Play();
			}
			else
			{
				PlayerBall->CanWalkInWater = true;
			}
		}
		else
		{
			APhysicsCube* Helper = Cast<APhysicsCube>(OtherActor);
			if (Helper)
			{
				if ((Helper->CubeStatus == CStatus::Iron && CurrentStatus == FluidStatus::CuSO4)
					|| (Helper->CubeStatus == CStatus::Zinc && CurrentStatus == FluidStatus::FeCl3)
					||(Helper->CubeStatus == CStatus::Copper && CurrentStatus == FluidStatus::ZnCl2))
				{
					LastStatus = CurrentStatus;
					CurrentStatus = FluidStatus::Water;
					ColorChangeTimer = ColorChangeBound;
					Helper->SetStatus(CStatus::Wood);
				}
			}
		}
	}
}

void ALake::OnEndOverlap(AActor* PrimitiveActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(OtherActor);
		if (PlayerBall)
		{
			PlayerBall->CanWalkInWater = false;
		}
	}
}