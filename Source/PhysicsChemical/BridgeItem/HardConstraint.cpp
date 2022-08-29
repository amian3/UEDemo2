// Fill out your copyright notice in the Description page of Project Settings.


#include "HardConstraint.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AHardConstraint::AHardConstraint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Game/Bridge/meshs/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Game/Bridge/meshs/Cylinder.Cylinder"));

	//Rt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rt"));
//	RootComponent = Rt;

	Mid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mid"));
	RootComponent = Mid;

	MidConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("MidConstraint"));
	MidConstraint->AttachTo(RootComponent);

	End1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("End1"));
	End1->AttachTo(RootComponent);
	End2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("End2"));
	End2->AttachTo(RootComponent);
	End1->SetStaticMesh(CylinderMesh.Object);
	End1->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	End2->SetStaticMesh(CylinderMesh.Object);
	End2->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	End1->SetSimulatePhysics(true);
	End2->SetSimulatePhysics(true);
	MidConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	MidConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	MidConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);


	MidConstraint->SetConstrainedComponents(End1, End1->GetFName(), End2, End2->GetFName());
	EndConstraint1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("EndConstraint1"));
	EndConstraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("EndConstraint2"));
	EndConstraint1->AttachTo(End1);
	EndConstraint2->AttachTo(End2);
	FConstrainComponentPropName Cur;
	Cur.ComponentName = End1->GetFName();
	FConstrainComponentPropName Cur1;
	Cur1.ComponentName = End2->GetFName();
	EndConstraint1->ComponentName1 = Cur;
	EndConstraint2->ComponentName1 = Cur1;



}

// Called when the game starts or when spawned
void AHardConstraint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHardConstraint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

