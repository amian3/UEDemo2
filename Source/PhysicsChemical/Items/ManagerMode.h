// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagerMode.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API AManagerMode : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Cube;
public:	
	// Sets default values for this actor's properties
	AManagerMode();

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName CorrespondingName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialParameterCollection* CorrespondingCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTimelineComponent* MaterialTimeLine;

	UStaticMeshComponent* GetStaticMesh();

	UFUNCTION()
		void OnMaterialTimelineFinished();
	UFUNCTION()
		void OnMaterialTimelineTick(const float Output);

	bool IsInChange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* FloatCurve;
};
