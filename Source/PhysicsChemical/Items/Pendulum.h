// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.h"
#include "Pendulum.generated.h"

UCLASS()
class PHYSICSCHEMICAL_API APendulum : public AInteractiveItem
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pendulum, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* EndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pendulum, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Boundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pendulum, meta = (AllowPrivateAccess = "true"))
		class UPhysicsConstraintComponent* Constraint;



public:	
	// Sets default values for this actor's properties
	APendulum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPhysicsConstraintComponent* GetConstraint();

	void InitConstraint(AActor* End);

	void DestoryConstraint();


	bool IsActivate = false;
	UFUNCTION(BlueprintCallable)
		bool IsAlreadyActivate();
	UFUNCTION(BlueprintCallable)
		void ActivatePendulum();
	UFUNCTION(BlueprintCallable)
		void DeactivatePendulum();
	UFUNCTION(BlueprintCallable)
		void OnClickPendulum();
};
