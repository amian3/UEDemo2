// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RebornManager.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API ARebornManager : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* DetectBox;

public:	
	// Sets default values for this actor's properties
	ARebornManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector RebornLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RebornPriority;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDetectBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
