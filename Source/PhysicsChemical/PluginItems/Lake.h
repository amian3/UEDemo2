// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaterBodyLakeActor.h"
#include "Math/Color.h"
#include "Lake.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class FluidStatus : uint8
{
	Water UMETA(DisplayName = "Water"),
	FeCl3 UMETA(DisplayName = "FeCl3"),
	ZnCl2 UMETA(DisplayName = "ZnCl2"),
	CuSO4 UMETA(DisplayName = "CuSO4"),
};
UCLASS()
class PHYSICSCHEMICAL_API ALake : public AWaterBodyLake
{
	GENERATED_BODY()

public:
	//FVector4 Water(10.0f, 150.0f, 350.0f, 8.0f);
	//FVector4 FeCl3(350.0f, 150.0f, 10.0f, 8.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FluidStatus CurrentStatus = FluidStatus::Water;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Liquid")
		TMap<FluidStatus, FLinearColor> ColorMap;

	int32 ColorChangeTimer = 0;
	int32 ColorChangeBound = 300;
	FluidStatus LastStatus;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	ALake();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnBeginOverlap(AActor* PrimitiveActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
		void OnEndOverlap(AActor* PrimitiveActor, AActor* OtherActor);


};
