// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MovePawn.generated.h"

UENUM(BlueprintType)
enum class MMouseStatus : uint8
{
	Normal				UMETA(DisplayName = "Normal"),
	HoldSth				UMETA(DisplayName = "HoldSth"),
	CreateConstraint	UMETA(DisplayName = "CreateConstraint"),
};


UCLASS()
class PHYSICSCHEMICAL_API AMovePawn : public APawn
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Ball;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPawnMovementComponent* MovementComp;

public:
	// Sets default values for this pawn's properties
	AMovePawn();

	void MoveForward(float Val);
	void MoveRight(float Val);
	void MousePressed();
	void MouseReleased();
	void MouseDelete();

	FVector CurrentForwardValue;
	FVector CurrentRightValue;
	FVector MinimumBound;
	FVector MaximumBound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		MMouseStatus MouseStatus = MMouseStatus::Normal;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AActor* CurrentCube;

	void BridgeConstraintPressed();
	void BridgeConstraintReleased();
	void BridgeSwitch();
	FHitResult FirstHitActor;
	FHitResult SecondHitActor;
	bool WithFirstActor = false;
	bool WithSecondActor = false;

	AActor* ItemManager;

	TArray<AActor*> ItemBuffer;
	void PInput();
};
