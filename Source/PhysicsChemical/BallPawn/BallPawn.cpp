// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "../Items/SpringItem.h"
#include "../Items/Pendulum.h"
#include "../Items/GravityBall.h"
#include "../Items/PhysicsCube.h"
#include "../Items/IceRoad.h"
#include "../BridgeItem/CuboidItem.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "BuoyancyComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "../BridgeItem/ItemManager.h"
#include "../BridgeItem/HardConstraint.h"
// Sets default values
ABallPawn::ABallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(AngularDamping);
	Ball->SetLinearDamping(LinearDamping);
	Ball->BodyInstance.MassScale = MassScale;
	Ball->BodyInstance.MaxAngularVelocity = MaxAngularVelocity;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	Effects = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Effects"));
	Effects->AttachTo(Ball);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	//SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1800.f;
//	SpringArm->bEnableCameraLag = false;
//	SpringArm->CameraLagSpeed = 3.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = false;
	
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	Buoyancy->AddCustomPontoon(100.0f, FVector(0.0f, 0.0f, 0.0f));
	//Buoyancy->SetCanBeActive(true);

	
	TimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	ReverseTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("ReverseTimeline"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/BallPawn/FreeToLockCurve"));
	check(Curve.Succeeded());
	FloatCurve = Curve.Object;
	static ConstructorHelpers::FObjectFinder<UCurveFloat> VanishCurve(TEXT("/Game/BallPawn/VanishCurve"));
	check(VanishCurve.Succeeded());
	VanishFloatCurve = VanishCurve.Object;
	UnLockMap.Add(ProperyStatus::Wood);
	UnLockMap[ProperyStatus::Wood] = true;
	UnLockMap.Add(ProperyStatus::Ice);
	UnLockMap[ProperyStatus::Ice] = false;
	UnLockMap.Add(ProperyStatus::Fire);
	UnLockMap[ProperyStatus::Fire] = false;
	UnLockMap.Add(ProperyStatus::Copper);
	UnLockMap[ProperyStatus::Copper] = false;
	UnLockMap.Add(ProperyStatus::Iron);
	UnLockMap[ProperyStatus::Iron] = false;
	UnLockMap.Add(ProperyStatus::Zinc);
	UnLockMap[ProperyStatus::Zinc] = false;


	MaterialTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("MaterialTimeline"));

	VanishTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("VanishTimeline"));
	CorrespondingCollection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), nullptr, TEXT("/Game/BallPawn/ForBallPawn.ForBallPawn")));


}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();

	//Cast<APlayerController>(GetWorld()->GetFirstPlayerController())->setcontrolplayer
	//Cast<APlayerController>(GetWorld()->GetFirstPlayerController())->setcontrolplayer
	//TimeLine->SetLooping(true);
	FOnTimelineFloat TimelineTickDelegate;
	TimelineTickDelegate.BindUFunction(this, "OnTimelineTick");
	TimeLine->AddInterpFloat(FloatCurve, TimelineTickDelegate);
	TimeLine->SetTimelineLengthMode(TL_LastKeyFrame);

	FOnTimelineFloat ReverseTimelineTickDelegate;
	ReverseTimelineTickDelegate.BindUFunction(this, "OnReverseTimelineTick");
	ReverseTimeLine->AddInterpFloat(FloatCurve, ReverseTimelineTickDelegate);
	ReverseTimeLine->SetTimelineLengthMode(TL_LastKeyFrame);


	FOnTimelineFloat MaterialTimelineTickDelegate;
	MaterialTimelineTickDelegate.BindUFunction(this, "OnMaterialTimelineTick");
	FOnTimelineEventStatic MaterialTimelineFinished;
	MaterialTimelineFinished.BindUFunction(this, "OnMaterialTimelineFinished");
	MaterialTimeLine->SetTimelineFinishedFunc(MaterialTimelineFinished);
	MaterialTimeLine->AddInterpFloat(FloatCurve, MaterialTimelineTickDelegate);
	MaterialTimeLine->SetTimelineLengthMode(TL_LastKeyFrame);

	FOnTimelineFloat VanishTimelineTickDelegate;
	VanishTimelineTickDelegate.BindUFunction(this, "OnVanishTimelineTick");
	FOnTimelineEventStatic VanishTimelineFinished;
	VanishTimelineFinished.BindUFunction(this, "OnVanishTimelineFinished");
	VanishTimeLine->SetTimelineFinishedFunc(VanishTimelineFinished);
	VanishTimeLine->AddInterpFloat(VanishFloatCurve, VanishTimelineTickDelegate);
	VanishTimeLine->SetTimelineLengthMode(TL_LastKeyFrame);
	
}

void ABallPawn::FindItemManager()
{
	TArray<AActor*> Cur;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemManager::StaticClass(), Cur);
	if (Cur.Num() != 0)
	{
		ItemManager = Cur[0];
		UE_LOG(LogTemp, Log, TEXT("Success"));
	}
}

void ABallPawn::OnVanishTimelineFinished()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(),
		CorrespondingCollection, TEXT("Vanish"), 1);
	SetActorLocation(ReBornLocation);
	Ball->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
	Ball->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
}

void ABallPawn::OnVanishTimelineTick(const float Output)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(),
		CorrespondingCollection, TEXT("Vanish"), 1 - Output);
}
void ABallPawn::OnMaterialTimelineFinished()
{
	IsInChange = false;
	BallStatus = TargetStatus;
}

void ABallPawn::OnMaterialTimelineTick(const float Output)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), 
		CorrespondingCollection, TEXT("Amount1"), 
		(1 - Output) * (float)CurrentMaterialA + Output * (float)TargetMaterialA);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), 
		CorrespondingCollection, TEXT("Amount2"), 
		(1 - Output) * (float)CurrentMaterialB + Output * (float)TargetMaterialB);
}

void ABallPawn::OnReverseTimelineTick(const float Output)
{
	SpringArm->TargetArmLength = (1 - Output) * CurrentSpringArmLength + Output * 1800.f;
	Camera->SetRelativeLocation((1 - Output) * CurrentRelativeLocation + Output * FVector(0.0f, 0.0f, 0.0f));
}
void ABallPawn::OnTimelineTick(const float Output)
{
	SpringArm->TargetArmLength = (1 - Output) * CurrentSpringArmLength + Output * TargetSpringArmLength;
	Camera->SetRelativeLocation((1 - Output) * CurrentRelativeLocation + Output * TargetRelativeLocation);

	
	//FRotator TempRotation = CurrentRelativeRotation + FRotator(180.0f, 180.0f, 180.0f);
	/*
	if ((TempRotation.Pitch > 360.0f ? TempRotation.Pitch - 360.0f : TempRotation.Pitch) > TargetRelativeRotation.Pitch)
	{
		if (CurrentRelativeRotation.Pitch < TargetRelativeRotation.Pitch)
		{
			TempRotation.Pitch = (1 - Output) * CurrentRelativeRotation.Pitch + Output * TargetRelativeRotation.Pitch;
		}
		else
		{
			TempRotation.Pitch = (1 - Output) * CurrentRelativeRotation.Pitch + Output * (TargetRelativeRotation.Pitch + 360.0f);
		}
	}
	else
	{
		if (CurrentRelativeRotation.Pitch > TargetRelativeRotation.Pitch)
		{
			TempRotation.Pitch = (1 - Output) * CurrentRelativeRotation.Pitch + Output * TargetRelativeRotation.Pitch;
		}
		else
		{
			TempRotation.Pitch = (1 - Output) * (CurrentRelativeRotation.Pitch + 360.0f) + Output * TargetRelativeRotation.Pitch;
		}
	}
	if ((TempRotation.Yaw > 360.0f ? TempRotation.Yaw - 360.0f : TempRotation.Yaw) > TargetRelativeRotation.Yaw)
	{
		if (CurrentRelativeRotation.Yaw < TargetRelativeRotation.Yaw)
		{
			TempRotation.Yaw = (1 - Output) * CurrentRelativeRotation.Yaw + Output * TargetRelativeRotation.Yaw;
		}
		else
		{
			TempRotation.Yaw = (1 - Output) * CurrentRelativeRotation.Yaw + Output * (TargetRelativeRotation.Yaw + 360.0f);
		}
	}
	else
	{
		if (CurrentRelativeRotation.Yaw > TargetRelativeRotation.Yaw)
		{
			TempRotation.Yaw = (1 - Output) * CurrentRelativeRotation.Yaw + Output * TargetRelativeRotation.Yaw;
		}
		else
		{
			TempRotation.Yaw = (1 - Output) * (CurrentRelativeRotation.Yaw + 360.0f) + Output * TargetRelativeRotation.Yaw;
		}
	}
	if ((TempRotation.Roll > 360.0f ? TempRotation.Roll - 360.0f : TempRotation.Roll) > TargetRelativeRotation.Roll)
	{
		if (CurrentRelativeRotation.Roll < TargetRelativeRotation.Roll)
		{
			TempRotation.Roll = (1 - Output) * CurrentRelativeRotation.Roll + Output * TargetRelativeRotation.Roll;
		}
		else
		{
			TempRotation.Roll = (1 - Output) * CurrentRelativeRotation.Roll + Output * (TargetRelativeRotation.Roll + 360.0f);
		}
	}
	else
	{
		if (CurrentRelativeRotation.Roll > TargetRelativeRotation.Roll)
		{
			TempRotation.Roll = (1 - Output) * CurrentRelativeRotation.Roll + Output * TargetRelativeRotation.Roll;
		}
		else
		{
			TempRotation.Roll = (1 - Output) * (CurrentRelativeRotation.Roll + 360.0f) + Output * TargetRelativeRotation.Roll;
		}
	}
*/
	//SpringArm->SetRelativeRotation(TempRotation);
	SpringArm->SetRelativeRotation((1 - Output) * CurrentRelativeRotation + Output * TargetRelativeRotation);
}




// Called every frame
void ABallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (BallStatus == ProperyStatus::Ice && Buoyancy->IsOverlappingWaterBody() && CanWalkInWater)
	{
		if (IceCreateTimer == 3)
		{
			IceCreateTimer = 0;
			FVector pos = GetActorLocation();
			pos.X = (int)(pos.X) / 50 * 50;
			pos.Y = (int)(pos.Y) / 50 * 50;
			pos.Z -= DisToG;
			AIceRoad* NewIceRoad = GetWorld()->SpawnActor<AIceRoad>(pos, FRotator::ZeroRotator);
		}
		else
		{
			IceCreateTimer += 1;
		}

	}
	if (MStatus == MoveStatus::PrepareShoot)
	{
		FVector2D Cur;
		Cast<APlayerController>(GetController())->GetMousePosition(Cur.X, Cur.Y);

		FVector2D Diff = Cur - SpringStartLocation2D;
		Diff.X = Diff.X > 0 ? log10(Diff.X + 10) * SpringMultiplier : -log10(-Diff.X + 10) * SpringMultiplier;
		Diff.Y = Diff.Y > 0 ? log10(Diff.Y + 10) * SpringMultiplier : -log10(-Diff.Y + 10) * SpringMultiplier;
		CurrentSpringDirection.Normalize();
		FVector Dir(Diff.X * CurrentSpringDirection.X, Diff.X * CurrentSpringDirection.Y, -Diff.Y);


		SetActorLocation(SpringStartLocation + Dir);
	}


}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ABallPawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ABallPawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABallPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABallPawn::MoveForward);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABallPawn::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABallPawn::Interact);
	PlayerInputComponent->BindAction("Mouse", IE_Released, this, &ABallPawn::MouseReleased);
	PlayerInputComponent->BindAction("Mouse", IE_Pressed, this, &ABallPawn::MousePressed);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABallPawn::SetFire);
	PlayerInputComponent->BindAction("Ice", IE_Pressed, this, &ABallPawn::SetIce);
	PlayerInputComponent->BindAction("Iron", IE_Pressed, this, &ABallPawn::SetIron);
	PlayerInputComponent->BindAction("Wood", IE_Pressed, this, &ABallPawn::SetWood);
	//PlayerInputComponent->BindAction("Zinc", IE_Pressed, this, &ABallPawn::SetZinc);
	//PlayerInputComponent->BindAction("Copper", IE_Pressed, this, &ABallPawn::SetCopper);
	PlayerInputComponent->BindAction("BridgeSwitch", IE_Pressed, this, &ABallPawn::BridgeSwitch);
//	PlayerInputComponent->BindAction("BridgeConstraint", IE_Pressed, this, &ABallPawn::BridgeConstraintPressed);
//	PlayerInputComponent->BindAction("BridgeConstraint", IE_Released, this, &ABallPawn::BridgeConstraintReleased);
}

void ABallPawn::SetFire()
{
	if (UnLockMap[ProperyStatus::Fire] && BallStatus != ProperyStatus::Fire && (!IsInChange) && !Buoyancy->IsOverlappingWaterBody())
	{
		IsInChange = true;
		TargetStatus = ProperyStatus::Fire;
		Buoyancy->Deactivate();
		CurrentMaterialA = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount1"));
		CurrentMaterialB = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount2"));
		TargetMaterialA = 1;
		TargetMaterialB = 0;
		MaterialTimeLine->SetPlaybackPosition(0.0f, false);
		MaterialTimeLine->Play();
		
	}
}


void ABallPawn::SetIron()
{
	if (UnLockMap[ProperyStatus::Iron] && BallStatus != ProperyStatus::Iron && (!IsInChange) && !Buoyancy->IsOverlappingWaterBody())
	{
		IsInChange = true;
		TargetStatus = ProperyStatus::Iron;
		Buoyancy->Deactivate();
		CurrentMaterialA = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount1"));
		CurrentMaterialB = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount2"));
		TargetMaterialA = 1;
		TargetMaterialB = 1;
		MaterialTimeLine->SetPlaybackPosition(0.0f, false);
		MaterialTimeLine->Play();
	}
}

void ABallPawn::SetIce()
{
	if (UnLockMap[ProperyStatus::Ice] && BallStatus != ProperyStatus::Ice && (!IsInChange) && !Buoyancy->IsOverlappingWaterBody())
	{
		IsInChange = true;
		TargetStatus = ProperyStatus::Ice;
		Buoyancy->Activate();
		CurrentMaterialA = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount1"));
		CurrentMaterialB = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount2"));
		TargetMaterialA = 0;
		TargetMaterialB = 0;
		MaterialTimeLine->SetPlaybackPosition(0.0f, false);
		MaterialTimeLine->Play();
	}
}

void ABallPawn::ForceSetIce()
{
	if (BallStatus != ProperyStatus::Ice)
	{
		IsInChange = true;
		TargetStatus = ProperyStatus::Ice;
		Buoyancy->Activate();
		CurrentMaterialA = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount1"));
		CurrentMaterialB = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount2"));
		TargetMaterialA = 0;
		TargetMaterialB = 0;
		MaterialTimeLine->SetPlaybackPosition(0.0f, false);
		MaterialTimeLine->Play();
	}
}


void ABallPawn::SetWood()
{
	if (UnLockMap[ProperyStatus::Wood] && BallStatus != ProperyStatus::Wood && (!IsInChange) && !Buoyancy->IsOverlappingWaterBody())
	{
		IsInChange = true;
		TargetStatus = ProperyStatus::Wood;
		Buoyancy->Activate();
		CurrentMaterialA = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount1"));
		CurrentMaterialB = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), CorrespondingCollection, TEXT("Amount2"));
		TargetMaterialA = 0;
		TargetMaterialB = 1;
		MaterialTimeLine->SetPlaybackPosition(0.0f, false);
		MaterialTimeLine->Play();
	}
}


void ABallPawn::MoveRight(float Val)
{
	if (Buoyancy->IsOverlappingWaterBody() && CanWalkInWater && BallStatus == ProperyStatus::Ice)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector Impulse = FVector(Val * Direction.X * WaterImpulse, Val * Direction.Y * WaterImpulse, 0.0f);
		Ball->AddImpulse(Impulse);
	}
	else if(!Buoyancy->IsOverlappingWaterBody())
	{
		if (CMode == CameraMode::Free)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			const FVector Torque = FVector(-Direction.Y * Val * RollTorque, Direction.X * Val * RollTorque, 0.0f);
			Ball->AddTorqueInRadians(Torque);
		}
		else if (CMode == CameraMode::Lock)
		{
			const FVector Torque = FVector(-CurrentRightDirection.Y * Val * RollTorque,
				CurrentRightDirection.X * Val * RollTorque, 0.0f);
			Ball->AddTorqueInRadians(Torque);
		}
		else if (CMode == CameraMode::Static)
		{
			const FVector Torque = FVector(-CurrentRightDirection.Y * Val * RollTorque,
				CurrentRightDirection.X * Val * RollTorque, 0.0f);
			Ball->AddTorqueInRadians(Torque);
		}
	}
}

void ABallPawn::MoveForward(float Val)
{
	if (Buoyancy->IsOverlappingWaterBody() && CanWalkInWater && BallStatus == ProperyStatus::Ice)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector Impulse = FVector(Val * Direction.X * WaterImpulse, Val * Direction.Y * WaterImpulse, 0.0f);
		Ball->AddImpulse(Impulse);
	}
	else if(!Buoyancy->IsOverlappingWaterBody())
	{
		if (CMode == CameraMode::Free)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			const FVector Torque = FVector(-Direction.Y * Val * RollTorque, Direction.X * Val * RollTorque, 0.0f);
			//const FVector Torque = FVector(0.0f, 1.0f * Val * RollTorque, 0.0f);
			Ball->AddTorqueInRadians(Torque);
		}
		else if (CMode == CameraMode::Lock)
		{
			const FVector Torque = FVector(-CurrentForwardDirection.Y * Val * RollTorque,
				CurrentForwardDirection.X * Val * RollTorque, 0.0f);
			Ball->AddTorqueInRadians(Torque);
		}
		else if (CMode == CameraMode::Static)
		{
			const FVector Torque = FVector(-CurrentForwardDirection.Y * Val * RollTorque,
				CurrentForwardDirection.X * Val * RollTorque, 0.0f);
			Ball->AddTorqueInRadians(Torque);
		}
	}
}

void ABallPawn::Jump()
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.0f, 0.0f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}

void ABallPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
}

UStaticMeshComponent* ABallPawn::GetStaticMesh()
{
	return Ball;
}

void ABallPawn::Interact()
{
	TArray<AActor*> ignoreChars;
	TArray<AActor*> destActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> destObjectTypes;
	destObjectTypes.Add((EObjectTypeQuery)::ECollisionChannel::ECC_WorldDynamic);
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), GetActorLocation(), 1500.0f,
		destObjectTypes, AInteractiveItem::StaticClass(), ignoreChars, destActors
	);
	if (destActors.Num() != 0)
	{
		APendulum* Pendulum = Cast<APendulum>(destActors[0]);
		if (Pendulum && Pendulum->IsActivate)
		{
			if (Pendulum->GetConstraint()->ConstraintActor2 == nullptr)
			{
				Pendulum->InitConstraint(this);
				FVector SpawnV = 2 * Ball->GetPhysicsLinearVelocity();
			//	SpawnV.X = fminf(3 * SpawnV.X, 30.0f);
			//	SpawnV.Y = fminf(3 * SpawnV.Y, 30.0f);
		//		SpawnV.Z = fminf(3 * SpawnV.Z, 30.0f);
				Ball->SetPhysicsLinearVelocity(SpawnV);
				UE_LOG(LogTemp, Log, TEXT("init"));
			}
			else
			{
				Pendulum->DestoryConstraint();
				UE_LOG(LogTemp, Log, TEXT("destory"));
			}
		}
		ASpringItem* Spring = Cast<ASpringItem>(destActors[0]);

		if (Spring && GetDistanceTo(Spring) < 200.0f)
		{
			if (Ball->IsSimulatingPhysics())
			{
				SetActorLocation(Spring->ShootLocation());
				Ball->SetSimulatePhysics(false);
				Spring->ActivateShootMode();
				MStatus = MoveStatus::InSpring;
				CurrentSpring = Spring;
				CurrentSpringDirection = Spring->Direction;
			}
			else
			{
				Ball->SetSimulatePhysics(true);
				Spring->DeactivateShootMode();
				MStatus = MoveStatus::Normal;
				CurrentSpring = nullptr;
			}
		}
		AGravityBall* GravityBall = Cast<AGravityBall>(destActors[0]);
		if (GravityBall)
		{
			GravityBall->OnClickGravityBall();
			for (int i = 1; i < destActors.Num(); ++i)
			{
				AGravityBall* OtherBall = Cast<AGravityBall>(destActors[i]);
				if (OtherBall)
				{
					OtherBall->OnClickGravityBall();
				}
			}
		}
	}
}

void ABallPawn::MouseReleased()
{
	ASpringItem* CurrentSp = Cast<ASpringItem>(CurrentSpring);
	if (MStatus == MoveStatus::PrepareShoot && CurrentSp)
	{
		CurrentSp->OnReleaseSpring();
		CurrentSp->DeactivateShootMode();
		MStatus = MoveStatus::Normal;
	}
	/*
	APhysicsCube* CurrentCb = Cast<APhysicsCube>(CurrentCube);
	if (MouseStatus == EMouseStatus::HoldSth && CurrentCb)
	{
		CurrentCb->OnReleasedCube();
		MouseStatus = EMouseStatus::Normal;
	}
	ACuboidItem* CurrentCd = Cast<ACuboidItem>(CurrentCuboid);
	if (MouseStatus == EMouseStatus::HoldSth && CurrentCd)
	{
		CurrentCd->OnReleasedCuboid();
		MouseStatus = EMouseStatus::Normal;
	}
*/
	AItemManager* IM = Cast<AItemManager>(ItemManager);
	if (IM)
	{
		IM->DisableManagerInput();

	}
}

void ABallPawn::MousePressed()
{
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel((ETraceTypeQuery)ECollisionChannel::ECC_Visibility, false, HitResult);
	if (HitResult.GetActor())
	{
		AGravityBall* GravityBall = Cast<AGravityBall>(HitResult.GetActor());
		if(GravityBall)
		{
			GravityBall->OnClickGravityBall();
		}
		APendulum* Pendulum = Cast<APendulum>(HitResult.GetActor());
		if (Pendulum)
		{
			Pendulum->OnClickPendulum();
		}
		ASpringItem* SpringItem = Cast<ASpringItem>(HitResult.GetActor());
		if (SpringItem && MStatus == MoveStatus::InSpring)
		{
			MStatus = MoveStatus::PrepareShoot;
			SpringItem->OnClickSpring();
			SpringStartLocation = GetActorLocation();
			Cast<APlayerController>(GetController())->GetMousePosition(SpringStartLocation2D.X, SpringStartLocation2D.Y);
		}
		/*
		APhysicsCube* PhysicsCube = Cast<APhysicsCube>(HitResult.GetActor());
		if (PhysicsCube)
		{
			PhysicsCube->OnClickCube();
			MouseStatus = EMouseStatus::HoldSth;
			CurrentCube = PhysicsCube;
		}
		ACuboidItem* CuboidItem = Cast<ACuboidItem>(HitResult.GetActor());
		if (CuboidItem)
		{
			if (HitResult.GetComponent()->GetFName() == TEXT("Cube"))
			{
				CuboidItem->OnClickCuboidItem();
			}
			else if (HitResult.GetComponent()->GetFName() == TEXT("CubeX"))
			{
				MouseStatus = EMouseStatus::HoldSth;
				CuboidItem->MoveByAxis(1);
				CurrentCuboid = CuboidItem;
			}
			else if (HitResult.GetComponent()->GetFName() == TEXT("CubeY"))
			{
				MouseStatus = EMouseStatus::HoldSth;
				CuboidItem->MoveByAxis(2);
				CurrentCuboid = CuboidItem;
			}
			else if (HitResult.GetComponent()->GetFName() == TEXT("CubeZ"))
			{
				MouseStatus = EMouseStatus::HoldSth;
				CuboidItem->MoveByAxis(3);
				CurrentCuboid = CuboidItem;
			}
		}*/
	}
	
	if (MouseStatus == EMouseStatus::Normal)
	{
		AItemManager* IM = Cast<AItemManager>(ItemManager);
		if (IM)
		{
			IM->EnableManagerInput();
			FHitResult Hit = IM->GetMouseSelectedActor();
			if (Hit.GetActor())
			{
				IM->AttachGizmo(Hit.GetActor());
			}
		}
	}/*
	else if (MouseStatus == EMouseStatus::CreateConstraint)
	{
		if (!FirstHitActor)
		{
			FirstHitActor = &HitResult;
		}
		else if (!SecondHitActor)
		{
			SecondHitActor = &HitResult;
			FVector Pos = 0.5f * FirstHitActor->GetActor()->GetActorLocation()
				+ 0.5f * SecondHitActor->GetActor()->GetActorLocation();
			AHardConstraint* Cur = GetWorld()->SpawnActor<AHardConstraint>(Pos, FRotator::ZeroRotator);
			Cur->GetEndConstraint1()->ConstraintActor2 = FirstHitActor->GetActor();
			Cur->GetEndConstraint2()->ConstraintActor2 = SecondHitActor->GetActor();
			Cur->GetEndConstraint1()->InitComponentConstraint();
			Cur->GetEndConstraint2()->InitComponentConstraint();
			BridgeConstraintReleased();
		}
	}*/
}

void ABallPawn::SetCurrentCameraProperty()
{
	CurrentRelativeLocation = Camera->GetRelativeLocation();

	if (GetController())
	{
		CurrentRelativeRotation = GetController()->GetControlRotation();
	}


	CurrentSpringArmLength = SpringArm->TargetArmLength;
}

void ABallPawn::SetTargetCameraProperty(float TargetArmLength, FVector RelativeLocation, FRotator RelativeRotation)
{
	TargetRelativeLocation = RelativeLocation;
	TargetRelativeRotation = RelativeRotation;
	TargetSpringArmLength = TargetArmLength;
	
}

void ABallPawn::BridgeSwitch()
{
	AItemManager* IM = Cast<AItemManager>(ItemManager);
	if (IM)
	{
		if (IM->IsGizmoAttached())
		{
			if (IM->GizmoMode == GizmoType::Translate)
			{
				IM->GizmoMode = GizmoType::Rotate;
			}
			else if (IM->GizmoMode == GizmoType::Rotate)
			{
				IM->GizmoMode = GizmoType::Translate;
			}
		}
	}
}
/*
void ABallPawn::BridgeConstraintReleased()
{
	//if (MouseStatus != EMouseStatus::HoldSth)
	//{
		MouseStatus = EMouseStatus::Normal;
		FirstHitActor = nullptr;
		SecondHitActor = nullptr;
	//}
}

void ABallPawn::BridgeConstraintPressed()
{
	//if (MouseStatus != EMouseStatus::HoldSth)
	//{
		MouseStatus = EMouseStatus::CreateConstraint;
	//}
}*/