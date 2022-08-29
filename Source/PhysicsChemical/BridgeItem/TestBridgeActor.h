// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestBridgeActor.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API ATestBridgeActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		TArray<class UStaticMeshComponent*> Ball;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		TArray<class UStaticMeshComponent*> Cylinder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		TArray<class UPhysicsConstraintComponent*> Constraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* EndConstraint1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* EndConstraint2;

public:	
	// Sets default values for this actor's properties
	ATestBridgeActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumPoint = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BreakThreshold = 100000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
