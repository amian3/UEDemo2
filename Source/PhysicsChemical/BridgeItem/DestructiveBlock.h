// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructiveBlock.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API ADestructiveBlock : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* Block;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		TArray<class UStaticMeshComponent*> Socket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		TArray<class UPhysicsConstraintComponent*> Constraint;

public:	
	// Sets default values for this actor's properties
	ADestructiveBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other,
		class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
		FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	bool InDestroy = false;
	float DestroyTimer = 0.0f;
	UPROPERTY(EditAnywhere)
		float DestroyBound = 10.0f;

};
