// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destructive.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API ADestructive : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* Item;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	//	class UBoxComponent* DetectBox;
public:	
	// Sets default values for this actor's properties
	ADestructive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*
	UFUNCTION()
		void OnDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
*/
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other,
		class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, 
		FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	bool InDestroy = false;
	float DestroyTimer = 0.0f;
	UPROPERTY(EditAnywhere)
		float DestroyBound = 10.0f;
};
