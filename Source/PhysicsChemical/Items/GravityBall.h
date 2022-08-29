// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.h"
#include "GravityBall.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API AGravityBall : public AInteractiveItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Boundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* DetectSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pendulum, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* Partic;

public:	
	// Sets default values for this actor's properties
	AGravityBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = Gravity)
		float GravityConstant = 4.0f * powf(10,12);

	UPROPERTY(EditAnywhere, Category = Gravity)
		float DistanceConstant = 1000.0f;

protected:
	bool WithGravity = false;
	UFUNCTION(BlueprintCallable)
		void ActivateGravity();
	UFUNCTION(BlueprintCallable)
		void DeactivateGravity();
	UFUNCTION(BlueprintCallable)
		bool IsGravity();
	void ApplyGravityToPlayer(AActor* Satellite);
	TArray<AActor*> Satellites;

	UFUNCTION()
		void OnDetectSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDetectSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION(BlueprintCallable)
		void OnClickGravityBall();
};
