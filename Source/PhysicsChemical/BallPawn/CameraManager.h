// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

UENUM(BlueprintType)
enum class CameraStatus : uint8
{
	Free	UMETA(DisplayName = "Free"),
	Lock	UMETA(DisplayName = "Lock"),
	Static  UMETA(DisplayName = "Static"),
};

UCLASS()
class PHYSICSCHEMICAL_API ACameraManager : public AActor
{
	GENERATED_BODY()
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* DetectBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
public:	
	// Sets default values for this actor's properties
	ACameraManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		CameraStatus CameraSta = CameraStatus::Free;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector AbsoluteLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector RelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TargetForwardDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TargetRightDirection;


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
