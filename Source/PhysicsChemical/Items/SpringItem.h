// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.h"
#include "SpringItem.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API ASpringItem : public AInteractiveItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Root;
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Back;
*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LegA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LegB;
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* Constraint;*/
public:	
	// Sets default values for this actor's properties
	ASpringItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*
	UFUNCTION(BlueprintCallable)
		UPhysicsConstraintComponent* GetConstraint();
*/
	UFUNCTION(BlueprintCallable)
		FVector ShootLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D Direction;
	/*
	UFUNCTION()
		void ApplySpringForce();

	UPROPERTY(EditAnywhere, Category = Hook)
		float HookParam = 1000.0f;
*/
	UPROPERTY(EditAnywhere)
		FVector2D StartLocation;

	UPROPERTY(EditAnywhere)
		FVector2D EndLocation;

	UFUNCTION(BlueprintCallable)
		void OnClickSpring();

	UFUNCTION(BlueprintCallable)
		void OnReleaseSpring();

	UPROPERTY(EditAnywhere, Category = Hook)
		float ShootVelocity = 1000.0f;

	UFUNCTION(BlueprintCallable)
		void ActivateShootMode();

	UFUNCTION(BlueprintCallable)
		void DeactivateShootMode();

	UPROPERTY(EditAnywhere, Category = Hook)
		bool InShootMode = false;

};
