module;
#include <glm/glm.hpp>
export module ThunderLib.Camera;

export namespace ThunderLib
{
	/// <summary>
	/// Creates an OrthographicCamera, good for 2D representations
	/// </summary>
	class OrthographicCamera
	{	
	public:
		/// <summary>
		/// Creates an OrthographicCamera from the provided bounds
		/// </summary>
		/// <param name="left">Left bound</param>
		/// <param name="right">Right bound</param>
		/// <param name="bottom">Bottom bound</param>
		/// <param name="top">Top bound</param>
		/// <remarks>
		/// Bounds define the coordinate space the camera can see
		/// anything outside of bounds will not be rendered.
		/// </remarks>
		OrthographicCamera(float left, float right, float bottom, float top);

		/// <summary>
		/// Redefines the camera projection/bounds
		/// </summary>
		/// <param name="left">Left bound</param>
		/// <param name="right">Right bound</param>
		/// <param name="bottom">Bottom bound</param>
		/// <param name="top">Top bound</param>
		void SetProjection(float left, float right, float bottom, float top);

		/// <summary>
		/// Gets the camera position in world space
		/// </summary>
		/// <returns>Vec3 with this camera's position in world space</returns>
		const glm::vec3& GetPosition() const { return position_; }

		/// <summary>
		/// Sets the camera position in world space
		/// </summary>
		/// <param name="position">Vec3 representing the new camera position in world space</param>
		void SetPosition(const glm::vec3& position) { position_ = position; RecalculateViewMatrix();}

		/// <summary>
		/// Gets the current camera rotation around the z axis
		/// </summary>
		/// <returns>Camera rotation angle in degrees</returns>
		float GetRotation() const { return rotation_; }

		/// <summary>
		/// Sets the camera rotation angle
		/// </summary>
		/// <param name="rotation">New camera rotation angle in degrees</param>
		void SetRotation(float rotation) { rotation_ = rotation; RecalculateViewMatrix(); }

		/// <summary>
		/// Gets the projection matrix of this camera
		/// </summary>
		/// <returns>An orthogonal projection matrix</returns>
		const glm::mat4& GetProjectionMatrix() const { return projection_matrix_; }
		
		/// <summary>
		/// Gets the view matrix of this camera
		/// </summary>
		/// <returns>View matrix of the camera</returns>
		const glm::mat4& GetViewMatrix() const { return view_matrix_; }

		/// <summary>
		/// Gets the view projection matrix of this camera that converts world coordinates to clip space
		/// </summary>
		/// <returns>View projection matrix of the camera</returns>
		const glm::mat4& GetViewProjectionMatrix() const { return view_projection_matrix_; }
	
	private:
		void RecalculateViewMatrix(); // Recalculates view matrix from the updated position and rotation of camera.
		
		glm::mat4 projection_matrix_;             // Converts view coords to clip space coords
		glm::mat4 view_matrix_ = glm::mat4(1.0f); // Converts world coords to view coords
		glm::mat4 view_projection_matrix_;        // Converts world coords to clip space coords

		glm::vec3 position_ = { 0.0f, 0.0f, 0.0f }; // World space position of camera
		float rotation_ = 0.0f; // Rotation angle around the z axis
	};
};