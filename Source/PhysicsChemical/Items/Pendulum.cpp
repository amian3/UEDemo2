// Fill out your copyright notice in the Description page of Project Settings.


#include "Pendulum.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
APendulum::APendulum()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndPoint"));
	RootComponent = EndPoint;

	Boundary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boundary"));
	Boundary->SetupAttachment(RootComponent);

	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	Constraint->SetupAttachment(RootComponent);


	Constraint->ConstraintActor1 = this;

	DeactivatePendulum();

	//Partic->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
}

// Called when the game starts or when spawned
void APendulum::BeginPlay()
{
	Super::BeginPlay();
	ActivatePendulum();

}

// Called every frame
void APendulum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UPhysicsConstraintComponent* APendulum::GetConstraint()
{
	return Constraint;
}

void APendulum::InitConstraint(AActor* End)
{
	if (IsActivate)
	{
		Constraint->ConstraintActor1 = this;
		Constraint->ConstraintActor2 = End;
		Constraint->InitComponentConstraint();

	}

}

void APendulum::DestoryConstraint()
{
	if (IsActivate)
	{
		Constraint->ConstraintActor2 = nullptr;
		Constraint->InitComponentConstraint();

	}
}

void APendulum::ActivatePendulum()
{
	EndPoint->SetVisibility(true);
	Boundary->SetVisibility(false);
	IsActivate = true;
}

void APendulum::DeactivatePendulum()
{
	EndPoint->SetVisibility(false);
	Boundary->SetVisibility(true);
	DestoryConstraint();
	IsActivate = false;
}

bool APendulum::IsAlreadyActivate()
{
	return IsActivate;
}

void APendulum::OnClickPendulum()
{
	if (IsActivate)
	{
		DeactivatePendulum();
	}
	else
	{
		ActivatePendulum();
	}
}