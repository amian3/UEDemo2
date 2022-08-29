// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicActor.h"
#include "BuoyancyComponent.h"
#include "PhysicsCube.generated.h"

UENUM(BlueprintType)
enum class CStatus : uint8
{
	Iron	UMETA(DisplayName = "Iron"),
	Copper	UMETA(DisplayName = "Copper"),
	Zinc	UMETA(DisplayName = "Zinc"),
	Wood	UMETA(DisplayName = "Wood"),
};

UCLASS()
class PHYSICSCHEMICAL_API APhysicsCube : public ABasicActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Cube;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UBuoyancyComponent* Buoyancy;

public:
	// Sets default values for this actor's properties
	APhysicsCube();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		CStatus CubeStatus = CStatus::Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<CStatus, UMaterialInterface*> MaterialMap;

	UFUNCTION(BlueprintCallable)
		void SetStatus(CStatus Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* GetStaticMesh();

	bool IsMoveable = false;
	UFUNCTION(BlueprintCallable)
		void OnClickCube();

	UFUNCTION(BlueprintCallable)
		void OnReleasedCube();
	/*
	UFUNCTION(BlueprintCallable)
		void  OnBeginCursor();

	UFUNCTION(BluepringCallable)
		void OnEndCursor();
*/
};
