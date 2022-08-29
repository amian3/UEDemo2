// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
// Sets default values
AItemManager::AItemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	EditRoot = CreateDefaultSubobject<USceneComponent>(TEXT("EditRoot"));
	RootComponent = EditRoot;
	TranslateRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TranslateRoot"));
	TranslateRoot->AttachTo(EditRoot);
	ArrowX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowX"));
	ArrowX->AttachTo(TranslateRoot);
	ArrowY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowY"));
	ArrowY->AttachTo(TranslateRoot);
	ArrowZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowZ"));
	ArrowZ->AttachTo(TranslateRoot);
	RotatorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RotatorRoot"));
	RotatorRoot->AttachTo(EditRoot);
	RotatorX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatorX"));
	RotatorX->AttachTo(RotatorRoot);
	RotatorY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatorY"));
	RotatorY->AttachTo(RotatorRoot);
	RotatorZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatorZ"));
	RotatorZ->AttachTo(RotatorRoot);

}

// Called when the game starts or when spawned
void AItemManager::BeginPlay()
{
	Super::BeginPlay();
	SetUpGizmoMaterials();
	EnableEditor(false);
}

// Called every frame
void AItemManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ParentActor)
	{
		UpdateGizmo();
		if (GizmoMode == GizmoType::Rotate)
		{
			UpdateRotatorRoot();
		}

		if (InputLogo)
		{
			if ((!IsInteractionInProgress()) && (!WasMouseDown))
			{
				WasMouseDown = true;
				SelectInteraction();
			}
		}
		else
		{
			if (WasMouseDown)
			{
				WasMouseDown = false;
				ClearInteraction();
			}
		}
		if (IsInteractionInProgress())
		{
			ProcessInteraction();
		}
	}

}

void AItemManager::SetUpGizmoMaterials()
{
	FHashedMaterialParameterInfo ParameterInfo(TEXT("Color"));
	MI_ArrowX = ArrowX->CreateDynamicMaterialInstance(0);
	MI_ArrowX->GetVectorParameterValue(ParameterInfo, ArrowXColor);
	MI_ArrowY = ArrowY->CreateDynamicMaterialInstance(0);
	MI_ArrowY->GetVectorParameterValue(ParameterInfo, ArrowYColor);
	MI_ArrowZ = ArrowZ->CreateDynamicMaterialInstance(0);
	MI_ArrowZ->GetVectorParameterValue(ParameterInfo, ArrowZColor);

	MI_RotateRoll = RotatorX->CreateDynamicMaterialInstance(0);
	MI_RotateRoll->GetVectorParameterValue(ParameterInfo, RotateRollColor);
	MI_RotatePitch = RotatorY->CreateDynamicMaterialInstance(0);
	MI_RotatePitch->GetVectorParameterValue(ParameterInfo, RotatePitchColor);
	MI_RotateYaw = RotatorZ->CreateDynamicMaterialInstance(0);
	MI_RotateYaw->GetVectorParameterValue(ParameterInfo, RotateYawColor);

}

void AItemManager::EnableEditor(bool Enable)
{
	SetActorHiddenInGame(!Enable);
}

void AItemManager::UpdateGizmo()
{
	float Dis = GetDistanceTo(
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager) / 700.0f;
	SetActorScale3D(FVector(Dis, Dis, Dis));
	if (GizmoMode == GizmoType::Translate)
	{
		TranslateRoot->SetWorldRotation(ParentActor->GetActorRotation());
	}
}

void AItemManager::UpdateRotatorRoot()
{
	FVector FCur = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation()
		- GetActorLocation();
	FRotator RCur = UKismetMathLibrary::MakeRotFromX(FCur);

	if (RCur.Yaw <= -90.0f && RCur.Yaw >= -180.0f)
	{
		RotatorOrientation = -180.0f;
	}
	else if (RCur.Yaw > -90.0f && RCur.Yaw <= 0.0f)
	{
		RotatorOrientation = -90.0f;
	}
	else if (RCur.Yaw > 0.0f && RCur.Yaw <= 90.0f)
	{
		RotatorOrientation = 0.0f;
	}
	else
	{
		RotatorOrientation = 90.0f;
	}
	RotatorRoot->SetRelativeRotation(FRotator(0.0f, 0.0f, RotatorOrientation));
}

bool AItemManager::IsInteractionInProgress()
{
	if (GizmoMode == GizmoType::Translate)
	{
		return TranslateDirection != TranslateType::None;
	}
	else if (GizmoMode == GizmoType::Rotate)
	{
		return RotateMode != RotateType::None;
	}
	return false;
}

void AItemManager::EnableManagerInput()
{
	InputLogo = true;
}

void AItemManager::DisableManagerInput()
{
	InputLogo = false;
}

void AItemManager::TranslateOnAxis(TranslateType& TT, FVector& ObjectLocation,
	float& SpeedMul, FVector2D MouseDelta,
	FTransform CameraTransform, FVector ProjectionAxis)
{
	FVector LocalMouseDirection;
	if (TT == TranslateType::None)
	{
		return;
	}
	else if (TT == TranslateType::X)
	{
		MouseDelta = SpeedMul * MouseDelta;
		LocalMouseDirection = FVector(0.0f, MouseDelta.X, MouseDelta.Y);
	}
	else if (TT == TranslateType::Y)
	{
		MouseDelta = SpeedMul * MouseDelta;
		LocalMouseDirection = FVector(MouseDelta.Y, MouseDelta.X, 0.0f);
	}
	else if (TT == TranslateType::Z)
	{
		MouseDelta = SpeedMul * MouseDelta;
		LocalMouseDirection = FVector(0.0f, MouseDelta.X, MouseDelta.Y);
	}
	ObjectLocation += UKismetMathLibrary::ProjectVectorOnToVector(
		UKismetMathLibrary::TransformDirection(CameraTransform, LocalMouseDirection), ProjectionAxis);
}

void AItemManager::RotateOnAxis(RotateType &RT, FRotator &ObjectRotation,
	float &SpeedMul, FVector2D MouseDelta, FVector MouseDirection)
{
	if (RT == RotateType::Roll)
	{
		float Angle = MouseDelta.Y * MouseDirection.Y * -1.0f * SpeedMul / 5.0f;
		FRotator Cur = UKismetMathLibrary::RotatorFromAxisAndAngle(FVector(1.0f, 0.0f, 0.0f), Angle);
		ObjectRotation = UKismetMathLibrary::ComposeRotators(ObjectRotation, Cur);
	}
	else if (RT == RotateType::Pitch)
	{
		float Angle = MouseDelta.Y * MouseDirection.X * SpeedMul / 5.0f;
		FRotator Cur = UKismetMathLibrary::RotatorFromAxisAndAngle(FVector(0.0f, 1.0f, 0.0f), Angle);
		ObjectRotation = UKismetMathLibrary::ComposeRotators(ObjectRotation, Cur);
	}
	else if (RT == RotateType::Yaw)
	{
		float Angle = MouseDelta.X * MouseDirection.Z * SpeedMul / 5.0f;
		FRotator Cur = UKismetMathLibrary::RotatorFromAxisAndAngle(FVector(0.0f, 0.0f, 1.0f), Angle);
		ObjectRotation = UKismetMathLibrary::ComposeRotators(ObjectRotation, Cur);
	}
}


RotateType AItemManager::GetRotationModeOverride()
{
	if (RotateMode == RotateType::Roll && (RotatorOrientation == 90.0f || RotatorOrientation == -90.0f))
	{
		return RotateType::Pitch;
	}
	else if (RotateMode == RotateType::Pitch && (RotatorOrientation == 90.0f || RotatorOrientation == -90.0f))
	{
		return RotateType::Roll;
	}
	else
	{
		return RotateMode;
	}
}

FVector AItemManager::GetTranslateProjectionAxis(TranslateType TT, AActor* Actor)
{
	if (TT == TranslateType::None)
	{
		return FVector(0.0f, 0.0f, 0.0f);
	}
	else if (TT == TranslateType::X)
	{
		return Actor->GetActorForwardVector();
	}
	else if (TT == TranslateType::Y)
	{
		return Actor->GetActorRightVector();
	}
	else if (TT == TranslateType::Z)
	{
		return Actor->GetActorUpVector();
	}
	return FVector(0.0f, 0.0f, 0.0f);
}

void AItemManager::ProcessInteraction()
{
	EComponentMobility::Type CurrentMobility;
	AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(ParentActor);
	if (StaticMeshActor)
	{
		CurrentMobility = StaticMeshActor->GetStaticMeshComponent()->Mobility;
		StaticMeshActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Type::Movable);
	}

	FVector ObjectLocation;
	FRotator ObjectRotation;
	FVector2D MouseDelta;
	FTransform CameraTransform;
	FVector ProjectionAxis;
	FVector MouseDirection;
//	RotateType RT;

	
	GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(MouseDelta.X, MouseDelta.Y);
	CameraTransform = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorTransform();
	ProjectionAxis = GetTranslateProjectionAxis(TranslateDirection, ParentActor);

	FVector2D MousePosition;
	FVector WorldLoc;
	GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y);
	GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLoc, MouseDirection);

	if (GizmoMode == GizmoType::Translate)
	{
		ObjectLocation = ParentActor->GetActorLocation();
		float multiplier = fmaxf((GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation()
			- GetActorLocation()).Size() / 2000.0f, 1.0f);
		TranslateOnAxis(TranslateDirection, ObjectLocation, SpeedMultiplier, multiplier * MouseDelta, CameraTransform, ProjectionAxis);
		ParentActor->SetActorLocation(ObjectLocation);
		SetActorLocation(ObjectLocation);
	}
	else if (GizmoMode == GizmoType::Rotate)
	{
		ObjectRotation = ParentActor->GetActorRotation();
		RotateType RT = GetRotationModeOverride();
		RotateOnAxis(RT, ObjectRotation, SpeedMultiplier, MouseDelta, MouseDirection);
		ParentActor->SetActorRotation(ObjectRotation);
	}
}

void AItemManager::ClearInteraction()
{
	if (GizmoMode == GizmoType::Translate)
	{
		if (TranslateDirection != TranslateType::None)
		{
			SetTranslateDirection(TranslateType::None);
		}
	}
	else if (GizmoMode == GizmoType::Rotate)
	{
		if (RotateMode != RotateType::None)
		{
			SetRotationMode(RotateType::None);
		}
	}
}

void AItemManager::SelectInteraction()
{
	float LocationX, LocationY;
	FVector WorldLocation;
	FVector WorldDirection;
	GetWorld()->GetFirstPlayerController()->GetMousePosition(LocationX, LocationY);
	GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(LocationX, LocationY, WorldLocation, WorldDirection);
	FVector Start = WorldLocation;
	FVector End = WorldLocation + 100000.0f * WorldDirection;
	
	UPrimitiveComponent* HitComponent = GizmoLineTrace(Start, End);
	
	if (GizmoMode == GizmoType::Translate)
	{
		if (HitComponent == ArrowX)
		{
			SetTranslateDirection(TranslateType::X);
		}
		else if (HitComponent == ArrowY)
		{
			SetTranslateDirection(TranslateType::Y);
		}
		else if (HitComponent == ArrowZ)
		{
			SetTranslateDirection(TranslateType::Z);
		}
	}
	else if (GizmoMode == GizmoType::Rotate)
	{
		if (HitComponent == RotatorX)
		{
			SetRotationMode(RotateType::Roll);
		}
		else if (HitComponent == RotatorY)
		{
			SetRotationMode(RotateType::Pitch);
		}
		else if (HitComponent == RotatorZ)
		{
			SetRotationMode(RotateType::Yaw);
		}
	}
}

UPrimitiveComponent* AItemManager::GizmoLineTrace(FVector Start, FVector End)
{
	FHitResult Result;
	bool HasHit;
	TArray<USceneComponent*> Child;
	EditRoot->GetChildrenComponents(true, Child);
	for (int i = 0; i < Child.Num(); ++i)
	{
		UPrimitiveComponent* Cur = Cast<UPrimitiveComponent>(Child[i]);
		if (Cur)
		{
			if (Cur->IsCollisionEnabled() && Cur->IsVisible())
			{
				FHitResult TempResult;
				if (Cur->LineTraceComponent(TempResult, Start, End, FCollisionQueryParams()))
				{
					if (TempResult.Time < Result.Time)
					{
						HasHit = true;
						Result = TempResult;
					}
				}
			}
		}
	}
	if (HasHit)
	{
		return Result.GetComponent();
	}
	else
	{
		return nullptr;
	}
}

void AItemManager::SetRotationMode(RotateType RT)
{
	if (RotateMode != RT)
	{
		ResetGizmoColors();
		RotateMode = RT;
		if (RotateMode == RotateType::None)
		{
			GetWorld()->GetFirstPlayerController()->ResetIgnoreLookInput();
		}
		else
		{
			GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(true);
			if (RotateMode == RotateType::Roll)
			{
				MI_RotateRoll->SetVectorParameterValue(TEXT("Color"), MouseDownColor);
			}
			else if (RotateMode == RotateType::Pitch)
			{
				MI_RotatePitch->SetVectorParameterValue(TEXT("Color"), MouseDownColor);
			}
			else if (RotateMode == RotateType::Yaw)
			{
				MI_RotateYaw->SetVectorParameterValue(TEXT("Color"), MouseDownColor);
			}
		}
	}
}

void AItemManager::SetTranslateDirection(TranslateType TT)
{
	if (TranslateDirection != TT)
	{
		ResetGizmoColors();
		TranslateDirection = TT;
		if (TranslateDirection == TranslateType::None)
		{
			GetWorld()->GetFirstPlayerController()->ResetIgnoreLookInput();
		}
		else
		{
			GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(true);
			if (TranslateDirection == TranslateType::X)
			{
				MI_ArrowX->SetVectorParameterValue(TEXT("Color"), MouseDownColor);
			}
			else if (TranslateDirection == TranslateType::Y)
			{
				MI_ArrowY->SetVectorParameterValue(TEXT("Color"), MouseDownColor);
			}
			else if (TranslateDirection == TranslateType::Z)
			{
				MI_ArrowZ->SetVectorParameterValue(TEXT("Color"), MouseDownColor);
			}
		}
	}
}

void AItemManager::ResetGizmoColors()
{
	MI_ArrowX->SetVectorParameterValue(TEXT("Color"), ArrowXColor);
	MI_ArrowY->SetVectorParameterValue(TEXT("Color"), ArrowYColor);
	MI_ArrowZ->SetVectorParameterValue(TEXT("Color"), ArrowZColor);
	MI_RotateRoll->SetVectorParameterValue(TEXT("Color"), RotateRollColor);
	MI_RotatePitch->SetVectorParameterValue(TEXT("Color"), RotatePitchColor);
	MI_RotateYaw->SetVectorParameterValue(TEXT("Color"), RotateYawColor);
}

FHitResult AItemManager::GetMouseSelectedActor()
{
	float LocationX, LocationY;
	FVector WorldLocation;
	FVector WorldDirection;
	GetWorld()->GetFirstPlayerController()->GetMousePosition(LocationX, LocationY);
	GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(LocationX, LocationY, WorldLocation, WorldDirection);
	FVector Start = WorldLocation;
	FVector End = WorldLocation + 100000.0f * WorldDirection;

	UPrimitiveComponent* HitComponent = GizmoLineTrace(WorldLocation, WorldLocation + 100000.0f * WorldDirection);
	FHitResult Hit;
	if (!HitComponent)
	{
		
		TArray<AActor*> IgnoreActors;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End,
			(ETraceTypeQuery)ECollisionChannel::ECC_Visibility, true,
			IgnoreActors, EDrawDebugTrace::Type::None, Hit, true))
		{
			//if(Hit.Actor->alled)
			return Hit;
		}
	}
	return Hit;

}

void AItemManager::AttachGizmo(AActor* Actor)
{
	if (Actor != ParentActor)
	{
		ParentActor = Actor;
		if (Actor)
		{
			EnableEditor(true);
			AStaticMeshActor* SM = Cast<AStaticMeshActor>(Actor);
			if (SM)
			{
				if (!SM->GetStaticMeshComponent()->IsSimulatingPhysics())
				{
					SetActorLocation(SM->GetActorLocation());
				}
			}
			else
			{
				//SetActorLocation(SM->GetActorLocation());
			}
		}
		else
		{
			EnableEditor(false);
		}
	}
}

bool AItemManager::IsGizmoAttached()
{
	return ParentActor != nullptr;
}