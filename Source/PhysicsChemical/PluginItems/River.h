// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaterBodyRiverActor.h"
#include "River.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICSCHEMICAL_API ARiver : public AWaterBodyRiver
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void FeCl3();

	UFUNCTION(BlueprintCallable)
		void Water();

	ARiver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
