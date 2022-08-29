// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardItem.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API ABoardItem : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class ULink* Link1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class ULink* Link2;

//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
//		class ULink* Link3;

//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	//	class ULink* Link4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* Constraint1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* Constraint2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* Constraint3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* Constraint4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Rt;
public:	
	// Sets default values for this actor's properties
	ABoardItem();

	FORCEINLINE class ULink* GetLink1() const
	{
		return Link1;
	}
	FORCEINLINE class ULink* GetLink2() const
	{
		return Link2;
	}
	FORCEINLINE class UStaticMeshComponent* GetMesh() const
	{
		return Mesh;
	}
	FORCEINLINE class UPhysicsConstraintComponent* GetConstraint3() const
	{
		return Constraint3;
	}
	FORCEINLINE class UPhysicsConstraintComponent* GetConstraint4() const
	{
		return Constraint4;
	}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
