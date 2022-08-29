// Fill out your copyright notice in the Description page of Project Settings.


#include "River.h"
#include "WaterSplineComponent.h"

ARiver::ARiver()
{

}

void ARiver::BeginPlay()
{
	for (int i = 0; i < GetWaterSplineMetadata()->WaterVelocityScalar.Points.Num(); ++i)
	{
	//	GetWaterSplineMetadata()->WaterVelocityScalar.Points[i].OutVal = -128.0f;;
	}
}


void ARiver::FeCl3()
{

	SetWaterMaterial(GetRiverToLakeTransitionMaterialInstance());

}
void ARiver::Water()
{
	SetWaterMaterial(GetRiverToOceanTransitionMaterialInstance());
}