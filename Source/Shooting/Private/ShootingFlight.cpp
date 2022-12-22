// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingFlight.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AShootingFlight::AShootingFlight()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹 �ڽ� ������Ʈ ����
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// ������ �浹 �ڽ� ������Ʈ�� ��Ʈ ������Ʈ�� ����
	RootComponent = BoxComponent; // == SetRootComponent(BoxComponent);
	// �ڽ� �ݸ��� ũ�⸦ (50, 50, 50) ���� ����
	BoxComponent->SetBoxExtent(FVector(50));


	// �޽� ������Ʈ ����
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// ������ �޽� ������Ʈ�� ��Ʈ ������Ʈ�� ���� ��ü�� ���
	MeshComponent->SetupAttachment(RootComponent);
	// Shape_Cube ������ ã�Ƽ� ������ ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeStaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// ���� ť�� ������ �ε��ϴµ� �����ߴٸ�
	if (CubeStaticMesh.Succeeded())
	{
		// �ε��� ������ �޽� ������Ʈ�� Static Mesh �׸� �Ҵ�
		MeshComponent->SetStaticMesh(CubeStaticMesh.Object);
	}
	// MeshComponent->SetWorldLocation(FVector(0, 0, -50));

}

// Called when the game starts or when spawned
void AShootingFlight::BeginPlay()
{
	Super::BeginPlay();
	
	// �÷��̾� ��Ʈ�ѷ��� ĳ����
	// APlayerController* PlayerController = Cast<APlayerController>(Controller);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem != nullptr)
		{
			SubSystem->AddMappingContext(IMC_MyMapping, 0);
		}
	}
}

// Called every frame
void AShootingFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// direction ���� ����ȭ
	// ����ڰ� �Է��� ������ �̵�
	// P = P0 + vt == SetActorLocation = GetActorLocation + (direction(����) * speed(ũ��)) * DeltaTime
	// ����*ũ�� = �ӵ�, Scalar = �ӵ�, Delta�� ��� ���� �帧 �ǹ�
	direction.Normalize(); // ���̰� 1�� ���� = ���� ����
	FVector dir = GetActorLocation() + (direction * MoveSpeed) * DeltaTime;
	SetActorLocation(dir);

}

// Called to bind functionality to input
void AShootingFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ������ UInputComponent* ������ UEnhancedInputComponent �� ��ȯ
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// �׼� �Լ��� ����
	EnhancedInputComponent->BindAction(IA_MoveHorizontal, ETriggerEvent::Triggered, this, &AShootingFlight::MoveHorizontal);
	EnhancedInputComponent->BindAction(IA_MoveHorizontal, ETriggerEvent::Completed, this, &AShootingFlight::MoveHorizontal);

	EnhancedInputComponent->BindAction(IA_MoveVertical, ETriggerEvent::Triggered, this, &AShootingFlight::MoveVertical);
	EnhancedInputComponent->BindAction(IA_MoveVertical, ETriggerEvent::Completed, this, &AShootingFlight::MoveVertical);

	EnhancedInputComponent->BindAction(IA_FireBullet, ETriggerEvent::Triggered, this, &AShootingFlight::FireBullet);

	//// Horizontal Axis �Է¿� �Լ��� ����
	//PlayerInputComponent->BindAxis("MoveHorizontal", this, &AShootingFlight::MoveHorizontal);
	//// Vertical Axis �Է¿� �Լ��� ����
	//PlayerInputComponent->BindAxis("MoveVertical", this, &AShootingFlight::MoveVertical);

	//// Fire Action �Է¿� �Լ��� ����
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShootingFlight::FireBullet);
}

//// �¿� �Է��� ���� �� ����� �Լ�
//void AShootingFlight::MoveHorizontal(float value) {
//	
//	// �Է� value ���
//	h = value;
//	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
//
//	direction.Y = h;
//}
//
//// ���� �Է��� ���� �� ����� �Լ�
//void AShootingFlight::MoveVertical(float value) {
//	
//	// �Է� value ���
//	v = value;
//	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), v);
//
//	direction.Z = v;
//}
//
//// ���콺 ���� ��ư�� ������ �� ����� �Լ�
//void AShootingFlight::FireBullet() {
//	
//	// Bullet ���� ��ġ�� ���� ���� ����
//	FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
//	// Bullet ���� �� ȸ������ ���� ���� ����
//	// �����Ϳʹ� �ٸ��� �ڵ忡���� FRotator(Pitch, Yaw, Roll) ����
//	FRotator SpawnRotation = FRotator(90.0f, 0, 0);
//	FActorSpawnParameters SpawnParam;
//	// ������Ʈ ���� �� ���� ��ġ�� �浹 ������ �ٸ� ������Ʈ�� ���� ��쿡 ��� �� ������ �����ϴ� �ɼ�
//	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	// Bullet�� ����
//	// Bullet Blueprint ����
//	GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);
//}

void AShootingFlight::MoveHorizontal(const FInputActionValue& value) {
	h = value.Get<float>();
	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
	direction.Y = h;
}

void AShootingFlight::MoveVertical(const FInputActionValue& value) {
	v = value.Get<float>();
	// UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
	direction.Z = v;
}

void AShootingFlight::FireBullet() {
		
	// Bullet ���� ��ġ�� ���� ���� ����
	FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
	// Bullet ���� �� ȸ������ ���� ���� ����
	// �����Ϳʹ� �ٸ��� �ڵ忡���� FRotator(Pitch, Yaw, Roll) ����
	FRotator SpawnRotation = FRotator(90.0f, 0, 0);
	FActorSpawnParameters SpawnParam;
	// ������Ʈ ���� �� ���� ��ġ�� �浹 ������ �ٸ� ������Ʈ�� ���� ��쿡 ��� �� ������ �����ϴ� �ɼ�
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Bullet�� ����
	// Bullet Blueprint ����
	GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);
}
