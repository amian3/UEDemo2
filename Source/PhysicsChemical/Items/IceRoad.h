// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceRoad.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API AIceRoad : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Road;
	
public:	
	// Sets default values for this actor's properties
	AIceRoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float DestroyTimer = 0.0f;
	UPROPERTY(EditAnywhere, Category = IceRoad)
		float DestroyBound = 5.0f;

};
