// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "../Items/PhysicsCube.h"
#include "../BridgeItem/ItemManager.h"
#include "../BridgeItem/HardConstraint.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "../BridgeItem/Link.h"
#include "../BridgeItem/BoardItem.h"
// Sets default values
AMovePawn::AMovePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	RootComponent = Ball;
	Ball->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
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

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = false;


	MovementComp = CreateDefaultSubobject<UPawnMovementComponent>(TEXT("MovementComp"));
}

// Called when the game starts or when spawned
void AMovePawn::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> Cur;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemManager::StaticClass(), Cur);
	if (Cur.Num() != 0)
	{
		ItemManager = Cur[0];
		UE_LOG(LogTemp, Log, TEXT("Success"));
	}
	
}

// Called every frame
void AMovePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentForwardValue.IsNearlyZero())
	{
		SetActorLocation(GetActorLocation() + MoveSpeed * CurrentForwardValue);
	}
	if (!CurrentRightValue.IsNearlyZero())
	{
		SetActorLocation(GetActorLocation() + MoveSpeed * CurrentRightValue);
	}

}

// Called to bind functionality to input
void AMovePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &AMovePawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMovePawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMovePawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMovePawn::MoveForward);
	PlayerInputComponent->BindAction("Mouse", IE_Released, this, &AMovePawn::MouseReleased);
	PlayerInputComponent->BindAction("Mouse", IE_Pressed, this, &AMovePawn::MousePressed);
	PlayerInputComponent->BindAction("MouseDelete", IE_Released, this, &AMovePawn::MouseDelete);
	PlayerInputComponent->BindAction("BridgeSwitch", IE_Pressed, this, &AMovePawn::BridgeSwitch);
	PlayerInputComponent->BindAction("BridgeConstraint", IE_Pressed, this, &AMovePawn::BridgeConstraintPressed);
	PlayerInputComponent->BindAction("BridgeConstraint", IE_Released, this, &AMovePawn::BridgeConstraintReleased);
	PlayerInputComponent->BindAction("PInput", IE_Released, this, &AMovePawn::PInput);
}

void AMovePawn::MoveRight(float Val)
{

	const FVector Direction = GetActorRightVector();
	CurrentForwardValue = Val * Direction;
//	const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);
	//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//AddMovementInput(Direction, Val);
}

void AMovePawn::MoveForward(float Val)
{
	
	const FVector Direction = GetActorForwardVector();
	CurrentRightValue = Val * Direction;
	//AddMovementInput(Direction, Val);
	//const FRotator YawRotation(0, Rotation.Yaw, 0);
	//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//AddMovementInput(Direction, Val);
}

void AMovePawn::MouseDelete()
{
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel((ETraceTypeQuery)ECollisionChannel::ECC_Visibility, false, HitResult);
	if (MouseStatus == MMouseStatus::Normal)
	{
		ABoardItem* ThisBoard = Cast<ABoardItem>(HitResult.GetActor());
		if (ThisBoard)
		{
			ThisBoard->Destroy();
		}
	}
}

void AMovePawn::MousePressed()
{
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel((ETraceTypeQuery)ECollisionChannel::ECC_Visibility, false, HitResult);
	/*if (HitResult.GetActor())
	{
		APhysicsCube* PhysicsCube = Cast<APhysicsCube>(HitResult.GetActor());
		if (PhysicsCube)
		{
			PhysicsCube->OnClickCube();
			MouseStatus = MMouseStatus::HoldSth;
			CurrentCube = PhysicsCube;
		}
	}*/
	if (MouseStatus == MMouseStatus::Normal)
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
		if (Cast<ULink>(HitResult.GetComponent()))//->StaticClass() == ULink::StaticClass())
		{
			ABoardItem* ThisBoard = Cast<ABoardItem>(HitResult.GetActor());
			if (ThisBoard)
			{
				FVector Pos = ThisBoard->GetActorLocation();
				float Rot = ThisBoard->GetActorRotation().GetEquivalentRotator().Yaw;
			//UE_LOG(LogTemp, Log, TEXT("%s,%s,%s"), ThisBoard->GetActorRotation().Pitch, ThisBoard->GetActorRotation().Yaw, ThisBoard->GetActorRotation().Roll)
				Pos = Pos - FVector(2100.0f * cosf(Rot), 0.0f, 2100.0f * sinf(-Rot));
				if (!ThisBoard->GetConstraint3()->IsActive())
				{
					ABoardItem* Board = GetWorld()->SpawnActor<ABoardItem>(Pos, FRotator::ZeroRotator);
					ThisBoard->GetConstraint3()->SetConstrainedComponents(ThisBoard->GetLink1(), ThisBoard->GetLink1()->GetFName(), Board->GetMesh(), Board->GetMesh()->GetFName());
					ThisBoard->GetConstraint4()->SetConstrainedComponents(ThisBoard->GetLink2(), ThisBoard->GetLink2()->GetFName(), Board->GetMesh(), Board->GetMesh()->GetFName());
					ItemBuffer.Add(Board);
				}
			}

		}
	}
	else if (MouseStatus == MMouseStatus::CreateConstraint)
	{
		if (!WithFirstActor)
		{
			WithFirstActor = true;
			FirstHitActor = HitResult;
		}
		else if (!WithSecondActor)
		{
			WithSecondActor = true;
			SecondHitActor = HitResult;
			FVector Pos = 0.5f * FirstHitActor.GetActor()->GetActorLocation()
				+ 0.5f * SecondHitActor.GetActor()->GetActorLocation();
			float Dis = (2 * Pos).Size();
			AHardConstraint* Cur = GetWorld()->SpawnActor<AHardConstraint>(Pos, FRotator::ZeroRotator);
			Cur->SetActorScale3D(FVector(1.0f, 1.0f, Dis / 100.0f));
			Cur->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));

			Cur->GetEndConstraint1()->ConstraintActor2 = FirstHitActor.GetActor();
			Cur->GetEndConstraint2()->ConstraintActor2 = SecondHitActor.GetActor();

			Cur->GetEndConstraint1()->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
			Cur->GetEndConstraint1()->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
			Cur->GetEndConstraint1()->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);
			Cur->GetEndConstraint2()->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
			Cur->GetEndConstraint2()->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
			Cur->GetEndConstraint2()->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

			Cur->GetEndConstraint1()->SetConstrainedComponents(
				Cur->GetEnd1(), Cur->GetEnd1()->GetFName(), FirstHitActor.GetComponent(), FirstHitActor.GetComponent()->GetFName());

			Cur->GetEndConstraint2()->SetConstrainedComponents(
				Cur->GetEnd2(), Cur->GetEnd2()->GetFName(), SecondHitActor.GetComponent(), SecondHitActor.GetComponent()->GetFName());
			BridgeConstraintReleased();
		}
	}
}

void AMovePawn::MouseReleased()
{
	AItemManager* IM = Cast<AItemManager>(ItemManager);
	if (IM)
	{
		IM->DisableManagerInput();

	}
}

void AMovePawn::PInput()
{
	for (int i = 0; i < ItemBuffer.Num(); ++i)
	{
		ABoardItem* cur = Cast<ABoardItem>(ItemBuffer[i]);
		if (cur)
		{
			cur->GetLink1()->SetSimulatePhysics(true);
			cur->GetLink2()->SetSimulatePhysics(true);
			cur->GetMesh()->SetSimulatePhysics(true);
		}
	}
	ItemBuffer.Empty();
}

void AMovePawn::BridgeConstraintReleased()
{
	//if (MouseStatus != EMouseStatus::HoldSth)
	//{
	MouseStatus = MMouseStatus::Normal;
	FirstHitActor;
	SecondHitActor;
	WithFirstActor = false;
	WithSecondActor = false;
	//}
}

void AMovePawn::BridgeConstraintPressed()
{
	//if (MouseStatus != EMouseStatus::HoldSth)
	//{
	MouseStatus = MMouseStatus::CreateConstraint;
	//}
}

void AMovePawn::BridgeSwitch()
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