// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemManager.generated.h"

UENUM(BlueprintType)
enum class GizmoType : uint8
{
	Translate	UMETA(DisplayName = "Translate"),
	Rotate		UMETA(DisplayName = "Rotate"),
};

UENUM(BlueprintType)
enum class TranslateType : uint8
{
	None	UMETA(DisplayName = "None"),
	X		UMETA(DisplayName = "X"),
	Y		UMETA(DisplayName = "Y"),
	Z		UMETA(DisplayName = "Z"),
};

UENUM(BlueprintType)
enum class RotateType : uint8
{
	None	UMETA(DisplayName = "None"),
	Roll	UMETA(DisplayName = "Roll"),
	Pitch	UMETA(DisplayName = "Pitch"),
	Yaw		UMETA(DisplayName = "Yaw"),
};


UCLASS()
class PHYSICSCHEMICAL_API AItemManager : public AActor
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		USceneComponent* EditRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TranslateRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ArrowX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ArrowY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ArrowZ;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RotatorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* RotatorX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* RotatorY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* RotatorZ;

public:	
	// Sets default values for this actor's properties
	AItemManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* ParentActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		GizmoType GizmoMode = GizmoType::Translate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TranslateType TranslateDirection = TranslateType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		RotateType RotateMode = RotateType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotatorOrientation;

	UFUNCTION(BlueprintCallable)
		RotateType GetRotationModeOverride();

	UFUNCTION(BlueprintCallable)
		FVector GetTranslateProjectionAxis(TranslateType TT, AActor* Actor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* MI_ArrowX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* MI_ArrowY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* MI_ArrowZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor ArrowXColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor ArrowYColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor ArrowZColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor MouseDownColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* MI_RotateRoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* MI_RotatePitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* MI_RotateYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor RotateRollColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor RotatePitchColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor RotateYawColor;

	UFUNCTION(BlueprintCallable)
		void SetUpGizmoMaterials();

	UFUNCTION(BlueprintCallable)
		void EnableEditor(bool Enable);

	UFUNCTION(BlueprintCallable)
		void UpdateGizmo();

	UFUNCTION(BlueprintCallable)
		void UpdateRotatorRoot();

	UFUNCTION(BlueprintCallable)
		void SelectInteraction();

	UFUNCTION(BlueprintCallable)
		void ClearInteraction();

	UFUNCTION(BlueprintCallable)
		void ProcessInteraction();

	UFUNCTION(BlueprintCallable)
		bool IsInteractionInProgress();

	UFUNCTION(BlueprintCallable)
		bool IsGizmoAttached();

	UFUNCTION(BlueprintCallable)
		UPrimitiveComponent* GizmoLineTrace(FVector Start, FVector End);

	UFUNCTION(BlueprintCallable)
		void TranslateOnAxis(TranslateType& TT, FVector& ObjectLocation, 
			float& SpeedMul, FVector2D MouseDelta, 
			FTransform CameraTransform, FVector ProjectionAxis);

	UFUNCTION(BlueprintCallable)
		void RotateOnAxis(RotateType &RT, FRotator &ObjectRotation,
			float &SpeedMul, FVector2D MouseDelta, FVector MouseDirection);

	UFUNCTION(BlueprintCallable)
		void ResetGizmoColors();

	UFUNCTION(BlueprintCallable)
		void SetTranslateDirection(TranslateType TT);

	UFUNCTION(BlueprintCallable)
		void SetRotationMode(RotateType RT);

	bool InputLogo = false;
	bool WasMouseDown = false;
	UFUNCTION(BlueprintCallable)
		void EnableManagerInput();

	UFUNCTION(BlueprintCallable)
		void DisableManagerInput();

	UFUNCTION(BlueprintCallable)
		FHitResult GetMouseSelectedActor();

	UFUNCTION(BlueprintCallable)
		void AttachGizmo(AActor* Actor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
