// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBridgeActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
ATestBridgeActor::ATestBridgeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Game/Bridge/meshs/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Game/Bridge/meshs/Cylinder.Cylinder"));
	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = Root;
	for (int i = 0; i < NumPoint; ++i)
	{
		Ball.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*FString::FromInt(i))));
		Ball[i]->SetStaticMesh(SphereMesh.Object);
		

		Ball[i]->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f * float(i)));
	//	Ball[i]->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
		Ball[i]->SetupAttachment(RootComponent);

		Ball[i]->SetSimulatePhysics(true);
	//	Ball[i]->SetMassOverrideInKg(NAME_None, 1.0f);
	//	Ball[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	for (int i = 0; i < NumPoint - 1; ++i)
	{
		Constraint.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(*FString::FromInt(i + NumPoint))));
		FConstrainComponentPropName Cur;
		Cur.ComponentName = FName(*FString::FromInt(i));
		FConstrainComponentPropName Cur1;
		Cur1.ComponentName = FName(*FString::FromInt(i + 1));

	//	Constraint[i]->ComponentName1 = Cur;
		//Constraint[i]->ComponentName2 = Cur1;
		//Constraint[i]->InitComponentConstraint();
		//Constraint[i]->SetRelativeLocation(0.5f * Ball[i]->GetRelativeLocation() + 0.5f + Ball[i + 1]->GetRelativeLocation());
		Constraint[i]->SetupAttachment(Ball[i]);
		Constraint[i]->SetWorldLocation(Ball[i]->GetComponentLocation());
		Constraint[i]->SetConstrainedComponents(Ball[i], Ball[i]->GetFName(), Ball[i + 1], Ball[i + 1]->GetFName());
	}

	for (int i = 0; i < NumPoint - 1; ++i)
	{
		Cylinder.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*FString::FromInt(i + 2 * NumPoint - 1))));
		Cylinder[i]->SetStaticMesh(CylinderMesh.Object);
		Cylinder[i]->SetupAttachment(Ball[i + 1]);
		Cylinder[i]->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Cylinder[i]->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.0f));
		Cylinder[i]->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
		Cylinder[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	Cylinder[i]->SetMassOverrideInKg(NAME_None, 0.001f);
	}

	EndConstraint1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("EndConstraint1"));
	EndConstraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("EndConstraint2"));
	
	FConstrainComponentPropName Cur;
	Cur.ComponentName = FName(*FString::FromInt(0));
	FConstrainComponentPropName Cur1;
	Cur1.ComponentName = FName(*FString::FromInt(NumPoint - 1));
	EndConstraint1->ComponentName1 = Cur;
	EndConstraint2->ComponentName1 = Cur1;
	
}

// Called when the game starts or when spawned
void ATestBridgeActor::BeginPlay()
{
	Super::BeginPlay();
	Constraint[NumPoint / 2]->SetLinearBreakable(true, BreakThreshold);
}

// Called every frame
void ATestBridgeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

