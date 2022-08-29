// Fill out your copyright notice in the Description page of Project Settings.


#include "SpringItem.h"
#include "../BallPawn/BallPawn.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
// Sets default values
ASpringItem::ASpringItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = Root;

	LegA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegA"));
	LegA->SetupAttachment(Root);
	LegB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegB"));
	LegB->SetupAttachment(Root);
	/*
	Back = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back"));
	Back->SetupAttachment(Root);
*/
	StaticItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticItem"));
	StaticItem->SetupAttachment(Root);
	/*
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	Constraint->SetupAttachment(Back);
	Constraint->ConstraintActor1 = this;
*/
}

// Called when the game starts or when spawned
void ASpringItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpringItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ApplySpringForce();
}
/*
void ASpringItem::ApplySpringForce()
{
	FVector dir = StaticItem->GetComponentLocation() - Back->GetComponentLocation();
	Back->AddForce(HookParam * dir);
}

UPhysicsConstraintComponent* ASpringItem::GetConstraint()
{
	return Constraint;
}
*/
FVector ASpringItem::ShootLocation()
{
	return StaticItem->GetComponentLocation();
}

void ASpringItem::OnClickSpring()
{
	if (InShootMode)
	{
		float mouseX, mouseY;
		GetWorld()->GetFirstPlayerController()->GetMousePosition(mouseX, mouseY);
		StartLocation.X = mouseX;
		StartLocation.Y = mouseY;
	}

}

void ASpringItem::OnReleaseSpring()
{
	if (InShootMode)
	{
		float mouseX, mouseY;
		GetWorld()->GetFirstPlayerController()->GetMousePosition(mouseX, mouseY);
		EndLocation.X = mouseX;
		EndLocation.Y = mouseY;
		FVector2D Diff = EndLocation - StartLocation;
		Diff.X = Diff.X > 0 ? log10(Diff.X + 10) * 10.0f : -log10(-Diff.X + 10) * 10.0f;
		Diff.Y = Diff.Y > 0 ? log10(Diff.Y + 10) * 10.0f : -log10(-Diff.Y + 10) * 10.0f;
		Direction.Normalize();
		FVector Dir(-Diff.X * Direction.X, -Diff.X * Direction.Y, Diff.Y);
		ABallPawn* PlayerBall = Cast<ABallPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (PlayerBall)
		{
			PlayerBall->GetBall()->SetSimulatePhysics(true);
			PlayerBall->GetBall()->SetPhysicsLinearVelocity(ShootVelocity * Dir);
		}
	}
}

void ASpringItem::ActivateShootMode()
{
	InShootMode = true;
}

void ASpringItem::DeactivateShootMode()
{
	InShootMode = false;
}