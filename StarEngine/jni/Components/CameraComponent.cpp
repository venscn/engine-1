#include "CameraComponent.h"
#include "../Graphics/GraphicsManager.h"
#include "../Objects/Object.h"
#include "../Input/InputManager.h"
#include "../Context.h"
#include "../Logger.h"
#include "../Helpers/HelpersMath.h"
#include "../Graphics/ScaleSystem.h"
#include "../Objects/BaseCamera.h"

namespace star
{
	CameraComponent::CameraComponent():
		BaseComponent(),
		m_Projection(),
		m_View(),
		m_ViewInverse(),
		m_FarPlane(100.0f),
		m_NearPlane(0.1f),
		m_FOV(static_cast<float>(PI/4.0f)),
		m_Size(0.0f),
		m_Zoom(1.0f),
		m_bIsActive(false),
		m_bPerspectiveProjection(false),
		m_AspectRatio(1.0f)
	{
	}

	CameraComponent::~CameraComponent(void)
	{
	}

	void CameraComponent::InitializeComponent()
	{
		m_AspectRatio = GraphicsManager::GetInstance()->GetWindowAspectRatio();
	
		//Calc perspective matrix
		if(m_bPerspectiveProjection)
		{
			m_Projection = MatrixPerspectiveFOV(m_FOV, 
												m_AspectRatio, 
												m_NearPlane, 
												m_FarPlane);
		}

		//Calc ortho matrix
		else
		{
			if(m_Size <= 0)
			{
				m_Size = static_cast<float>(GraphicsManager::GetInstance()->GetWindowHeight());
			}

			m_Projection = MatrixOrtho(m_Size * m_AspectRatio * m_Zoom,
										m_Size * m_Zoom, 
										m_NearPlane, 
										m_FarPlane);
		}
	}

	void CameraComponent::Update(const Context& context)
	{
#ifdef DESKTOP
		if(m_Size != GraphicsManager::GetInstance()->GetWindowHeight() || 
			m_Size * m_AspectRatio != GraphicsManager::GetInstance()->GetWindowWidth())
		{
			m_AspectRatio = GraphicsManager::GetInstance()->GetWindowAspectRatio();
			if(m_bPerspectiveProjection)
			{
				m_Projection = MatrixPerspectiveFOV(m_FOV, 
													m_AspectRatio, 
													m_NearPlane, 
													m_FarPlane);
			}
			else
			{
				m_Size = static_cast<float>(GraphicsManager::GetInstance()->GetWindowHeight());
				m_Projection = MatrixOrtho(m_Size * m_AspectRatio * m_Zoom, 
										   m_Size * m_Zoom, 
										   m_NearPlane, 
										   m_FarPlane);
			}
		}
#endif
#ifdef STAR2D
		auto pos = m_pParentObject->GetTransform()->GetWorldPosition();
		vec3 vEyePt = pos.pos3D();
#else
		vec3 vEyePt = m_pParentObject->GetTransform()->GetWorldPosition();
#endif
		vec3 vLookat, vUpVec;
		mat4x4 rotTransform;
	
#ifdef STAR2D
		float rotDegrees = m_pParentObject->GetTransform()->GetWorldRotation();
		rotDegrees = RadiansToDegrees(rotDegrees);
		quat rotation(vec3(0, 0, rotDegrees));
#else
		quat rotation = m_pParentObject->GetTransform()->GetWorldRotation();
#endif
		rotTransform = glm::toMat4(rotation);

		//Only vec4 * mat4x4  possible
		vec4 vLookTemp = vec4(0,0,1,0) * TransposeMatrix(rotTransform);
		vec4 vUpVecTemp = vec4(0,1,0,0) * TransposeMatrix(rotTransform);

		//put them into a vec3
		vLookat = vec3(vLookTemp.x, vLookTemp.y, vLookTemp.z);
		vUpVec = vec3(vUpVecTemp.x, vUpVecTemp.y, vUpVecTemp.z);

		//Calculate the viewmatrix and inverse
		m_View = MatrixLookAt(vEyePt, (vEyePt + vLookat), vUpVec);
		m_ViewInverse = TransposeMatrix(m_View);
	}

	void CameraComponent::Draw()
	{

	}

	void CameraComponent::SetFieldOfView(float fov)
	{
		m_FOV = fov;
	}

	void CameraComponent::SetOrthoSize(float size)
	{
		m_Size = size;
	}

	void CameraComponent::SetNearClippingPlane(float nearPlane)
	{
		m_NearPlane = nearPlane;
	}

	void CameraComponent::SetFarClippingPlane(float farPlane)
	{
		m_FarPlane = farPlane;
	}
		
	void CameraComponent::SetActive()
	{
		auto scene = GetGameScene();
	
		if(scene == nullptr)
		{
			Logger::GetInstance()->Log( 
				LogLevel::Error,
				_T("Can't set camera active, add this camera to a scene first!")
				);
		}
		else
		{
			scene->SetActiveCamera(dynamic_cast<BaseCamera*>(m_pParentObject));
		}
	}

	bool CameraComponent::IsActive() const
	{
		return m_bIsActive;
	}

	void CameraComponent::Activate()
	{
		m_bIsActive = true;
	}
	
	void CameraComponent::Deactivate()
	{
		m_bIsActive = false;
	}

	void CameraComponent::SetZoom(float zoom)
	{
		m_Zoom = zoom;
		m_Projection = MatrixOrtho(m_Size * m_AspectRatio * m_Zoom, 
									m_Size * m_Zoom, 
									m_NearPlane, 
									m_FarPlane);
	}

	float CameraComponent::GetZoom() const
	{
		return m_Zoom;
	}

	const mat4x4 & CameraComponent::GetView() const
	{
		return m_View;
	}

	const mat4x4 & CameraComponent::GetProjection() const
	{
		return m_Projection;
	}

	const mat4x4 & CameraComponent::GetViewInverse() const
	{
		return m_ViewInverse;
	}

	mat4x4 CameraComponent::GetProjectionViewInvesre() const
	{
		return m_Projection * m_ViewInverse;
	}

	mat4x4 CameraComponent::MatrixPerspectiveFOV(float FovY, float ratio, float nearPlane, float farPlane)
	{
		float viewSpaceWidth, viewSpaceHeight;
		
		viewSpaceHeight = glm::cot(FovY/2);
		viewSpaceWidth = viewSpaceHeight * ratio;

		mat4x4 matPerspective 
		(
		viewSpaceWidth, 0, 0, 0,
		0, viewSpaceHeight, 0, 0,
		0, 0, farPlane / (farPlane - nearPlane), 1,
		0, 0, -nearPlane * farPlane / (farPlane - nearPlane), 0
		);

		return matPerspective;
	}

	mat4x4 CameraComponent::MatrixOrtho(float width, float height, float nearPlane, float farPlane)
	{
		//opengl standard is -1 to 1 --> 2 width
		mat4x4 matOrtho
		(
		2/width, 0, 0, -1,
		0, 2/height, 0, -1,
		0, 0, 1/(farPlane - nearPlane), 0,
		0, 0, nearPlane/(nearPlane - farPlane), 1
		);

		return matOrtho;
	}

	mat4x4 CameraComponent::MatrixLookAt(const vec3& eye, const vec3& at, const vec3& up)
	{
		vec3 xAxis, yAxis, zAxis;

		zAxis = glm::normalize(at - eye);
		xAxis = glm::normalize(glm::cross(up, zAxis));
		yAxis = glm::cross(zAxis, xAxis);
		
		mat4x4 matLookAt 
		(
			 xAxis.x, yAxis.x, zAxis.x, 0,
			 xAxis.y, yAxis.y, zAxis.y, 0,
			 xAxis.z, yAxis.z, zAxis.z, 0,
			-glm::dot(xAxis, eye),  -glm::dot(yAxis, eye),  -glm::dot(zAxis, eye),  1
		);

		return matLookAt;
	}
}
