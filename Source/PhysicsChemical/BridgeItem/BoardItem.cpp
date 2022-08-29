// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardItem.h"
#include "Link.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
// Sets default values
ABoardItem::ABoardItem()
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Bridge/meshs/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Game/Bridge/meshs/Sphere.Sphere"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Rt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rt"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetStaticMesh(CubeMesh.Object);
	Mesh->SetRelativeScale3D(FVector(20.0f, 10.0f, 1.0f));
	RootComponent = Rt;

	Link1 = CreateDefaultSubobject<ULink>(TEXT("Link1"));
	Link2 = CreateDefaultSubobject<ULink>(TEXT("Link2"));
//	Link3 = CreateDefaultSubobject<ULink>(TEXT("Link3"));
	//Link4 = CreateDefaultSubobject<ULink>(TEXT("Link4"));
	
	Link1->SetStaticMesh(SphereMesh.Object);
	Link2->SetStaticMesh(SphereMesh.Object);

	Mesh->AttachTo(Rt);
	Link1->AttachTo(Rt);
	Link2->AttachTo(Rt);
	//Link3->AttachTo(Rt);
	//Link4->AttachTo(Rt);
	Mesh->SetRelativeLocation(FVector(-950.0f, 0.0f, 0.0f));
	Link1->SetRelativeLocation(FVector(-2000.0f, -450.0f, 0.0f));
	Link2->SetRelativeLocation(FVector(-2000.0f, 450.0f, 0.0f));

	Constraint1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint1"));
	Constraint1->AttachTo(Link1);
	Constraint1->SetConstrainedComponents(Link1, Link1->GetFName(), Mesh, Mesh->GetFName());
	Constraint1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	Constraint1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	Constraint1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	Constraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint2"));
	Constraint2->AttachTo(Link2);
	Constraint2->SetConstrainedComponents(Link2, Link2->GetFName(), Mesh, Mesh->GetFName());
	Constraint2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	Constraint2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	Constraint2->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	
	Constraint3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint3"));
	Constraint3->AttachTo(Link1);
	//Constraint3->SetConstrainedComponents(Link3, Link3->GetFName(), Mesh, Mesh->GetFName());
	//Constraint3->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	//Constraint3->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	//Constraint3->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	Constraint4 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint4"));
	Constraint4->AttachTo(Link2);
	//Constraint4->SetConstrainedComponents(Link4, Link4->GetFName(), Mesh, Mesh->GetFName());
	//Constraint4->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	//Constraint4->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	//Constraint4->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);
//	Link1->SetSimulatePhysics(true);
	//Link2->SetSimulatePhysics(true);
	//Mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABoardItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoardItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

