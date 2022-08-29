// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BuoyancyComponent.h"
#include "BallPawn.generated.h"

UENUM(BlueprintType)
enum class ProperyStatus : uint8
{
	Iron	UMETA(DisplayName = "Iron"),
	Copper	UMETA(DisplayName = "Copper"),
	Zinc	UMETA(DisplayName = "Zinc"),
	Fire	UMETA(DisplayName = "Fire"),
	Wood	UMETA(DisplayName = "Wood"),
	Ice		UMETA(DisplayName = "Ice"),
};

UENUM(BlueprintType)
enum class MoveStatus : uint8
{
	Normal			UMETA(DisplayName = "Normal"),
	InSpring		UMETA(DisplayName = "InSpring"),
	PrepareShoot	UMETA(DisplayName = "PrepareShoot"),
	InWater			UMETA(DisplayName = "InWater"),
};

UENUM(BlueprintType)
enum class EMouseStatus : uint8
{
	Normal				UMETA(DisplayName = "Normal"),
	HoldSth				UMETA(DisplayName = "HoldSth"),
	CreateConstraint	UMETA(DisplayName = "CreateConstraint"),
};

UENUM(BlueprintType)
enum class CameraMode : uint8
{
	Free UMETA(DisplayName = "Free"),
	Lock UMETA(DisplayName = "Lock"),
	Static UMETA(DisplayName = "Static"),
};
UCLASS()
class PHYSICSCHEMICAL_API ABallPawn : public APawn
{
	GENERATED_BODY()
	//实例, 弹簧臂以及摄像机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Ball;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Effects;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		class UBuoyancyComponent* Buoyancy;

public:
	bool CanWalkInWater = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ProperyStatus, UMaterialInterface*> MaterialMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* TransferMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* ItemManager;

	UFUNCTION()
		void FindItemManager();
		
public:
	// Sets default values for this pawn's properties
	ABallPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ProperyStatus BallStatus = ProperyStatus::Wood;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		MoveStatus MStatus = MoveStatus::Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMouseStatus MouseStatus = EMouseStatus::Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		CameraMode CMode = CameraMode::Free;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTimelineComponent* TimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* FloatCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* VanishFloatCurve;
	UFUNCTION()
		void OnTimelineTick(const float Output);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTimelineComponent* ReverseTimeLine;

	UFUNCTION()
		void OnReverseTimelineTick(const float Output);



	//float CameraChangeTimer = 0.f;
	//float CameraChangeBound = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector CurrentRelativeLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator CurrentRelativeRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentSpringArmLength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TargetRelativeLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator TargetRelativeRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TargetSpringArmLength;

	void SetCurrentCameraProperty();
	void SetTargetCameraProperty(float TargetArmLength, FVector RelativeLocation, FRotator RelativeRotation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)//在锁定和静态相机模式下使用
		FVector CurrentForwardDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector CurrentRightDirection;

	UPROPERTY(EditAnywhere, Category = Ball)
		float DisToG = 80.0f;
	int32 IceCreateTimer = 3;
	UPROPERTY(EditAnywhere, Category = Ball)
		float JumpImpulse = 1000.0f;

	UPROPERTY(EditAnywhere, Category = Ball)
		float WaterImpulse = 100.0f;

	UPROPERTY(EditAnywhere, Category = Ball)
		float RollTorque = 500000.0f;

	bool bCanJump = true;
	
	void SetFire();
	void SetIron();
	void SetWood();
	void SetIce();
	void ForceSetIce();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ProperyStatus, bool> UnLockMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTimelineComponent* MaterialTimeLine;

	int8 TargetMaterialA;
	int8 TargetMaterialB;
	int8 CurrentMaterialA;
	int8 CurrentMaterialB;
	UMaterialParameterCollection* CorrespondingCollection;
	bool IsInChange = false;
	ProperyStatus TargetStatus;

	UFUNCTION()
		void OnMaterialTimelineFinished();
	UFUNCTION()
		void OnMaterialTimelineTick(const float Output);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTimelineComponent* VanishTimeLine;

	UFUNCTION()
		void OnVanishTimelineTick(const float Output);

	UFUNCTION()
		void OnVanishTimelineFinished();


protected://基于物理的移动组件

	//void Turn(float Rate);
	//void LookUp(float Rate);
	//UFUNCTION(BlueprintCallable)
	void MoveRight(float Val);

	void MoveForward(float Val);

	//UFUNCTION(BlueprintCallable)
	void Jump();

//	void BridgeConstraintPressed();
	//void BridgeConstraintReleased();

	//FHitResult* FirstHitActor;
	//FHitResult* SecondHitActor;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	//void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

public://实例, 弹簧臂以及摄像机的Get方法

	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE class UTimelineComponent* GetTimeline() const { return TimeLine; }
	FORCEINLINE class UTimelineComponent* GetReverseTimeline() const { return ReverseTimeLine; }
	FORCEINLINE class UTimelineComponent* GetVanishTimeline() const { return VanishTimeLine; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = PhysicsProperty)
		float AngularDamping = 0.2f;

	UPROPERTY(EditAnywhere, Category = PhysicsProperty)
		float LinearDamping = 0.2f;

	UPROPERTY(EditAnywhere, Category = PhysicsProperty)
		float MassScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = PhysicsProperty)
		float MaxAngularVelocity = 200.0f;

	UFUNCTION(BlueprintCallable)
		UStaticMeshComponent* GetStaticMesh();

	UFUNCTION(BlueprintCallable)
		void Interact();

	UFUNCTION(BlueprintCallable)
		void BridgeSwitch();

	UFUNCTION(BlueprintCallable)
		void MouseReleased();

	UFUNCTION(BlueprintCallable)
		void MousePressed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector ReBornLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ReBornPriority;

	AActor* CurrentSpring;

//	AActor* CurrentCube;

	//AActor* CurrentCuboid;

	FVector2D SpringStartLocation2D;
	FVector SpringStartLocation;
	FVector2D CurrentSpringDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpringMultiplier = 50.0f;


};
