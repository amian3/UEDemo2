// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HardConstraint.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API AHardConstraint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* End1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* End2;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	//	class UStaticMeshComponent* Rt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* MidConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* EndConstraint1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* EndConstraint2;
public:	
	// Sets default values for this actor's properties
	AHardConstraint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UPhysicsConstraintComponent* GetEndConstraint1() const 
	{ 
		return EndConstraint1; 
	}
	FORCEINLINE class UPhysicsConstraintComponent* GetEndConstraint2() const
	{ 
		return EndConstraint2; 
	}

	FORCEINLINE class UStaticMeshComponent* GetEnd1() const
	{
		return End1;
	}
	FORCEINLINE class UStaticMeshComponent* GetEnd2() const
	{
		return End2;
	}
};
