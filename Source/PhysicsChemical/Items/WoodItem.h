// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WoodItem.generated.h"

UENUM(BlueprintType)
enum class BurstStatus : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Prepare		UMETA(DisplayName = "Prepare"),
	BroadCast	UMETA(DisplayName = "BroadCast"),
};
/*
UENUM(BlueprintType)
enum class ProperyStatus : uint8
{
	Iron UMETA(DisplayName = "Iron"),
	Fire UMETA(DisplayName = "Fire"),
	Wood UMETA(DisplayName = "Wood"),
};
*/

UCLASS()
class PHYSICSCHEMICAL_API AWoodItem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Cube;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Block;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* DetectBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* FireEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* SmokeEffect;
	
public:	
	// Sets default values for this actor's properties
	AWoodItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float BurstOutTimer = 0.0f;
	float BroadCastTimer = 0.0f;
	float StartBurnTimer = 0.0f;
	BurstStatus BStatus = BurstStatus::Normal;
	UPROPERTY(EditAnywhere, Category = WoodItem)
		float BurstOutBound = 3.0f;
	UPROPERTY(EditAnywhere, Category = WoodItem)
		float BroadCastBound = 2.0f;
	UPROPERTY(EditAnywhere, Category = WoodItem)
		float StartBurnBound = 1.0f;

	void SetBurn();

	UFUNCTION()
		void OnDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDetectBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
