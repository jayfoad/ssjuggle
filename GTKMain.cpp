#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>

#include "Canvas.h"
#include "Juggler.h"
#include "Siteswap.h"

class CairoCanvas : public Canvas
{
	Cairo::RefPtr<Cairo::Context> context;

public:
	CairoCanvas(Cairo::RefPtr<Cairo::Context> c) :
		context(c)
	{
		context->set_source_rgb(1.0, 1.0, 1.0);
		context->paint();

		context->set_source_rgb(0.0, 0.0, 0.0);
	}

	virtual void moveTo(float x, float y)
	{
		context->move_to(x, y);
	}

	virtual void lineTo(float x, float y)
	{
		context->line_to(x, y);
	}

	virtual void circle(float x, float y, float r)
	{
		context->begin_new_sub_path();
		context->arc(x, y, r, 0, 2 * M_PI);
	}

	virtual void stroke()
	{
		context->stroke();
	}
};

class MyDrawingArea : public Gtk::DrawingArea
{
	const Juggler& juggler;

public:
	MyDrawingArea(const Juggler& j) : juggler(j) {}

protected:
	virtual bool on_expose_event(GdkEventExpose* event)
	{
		Glib::RefPtr<Gdk::Window> window = get_window();
		if (!window)
			return true;

		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		Cairo::RefPtr<Cairo::Context> context = window->create_cairo_context();

		if (event)
		{
			context->rectangle(event->area.x, event->area.y,
				event->area.width, event->area.height);
			context->clip();
		}

		{
			float left;
			float right;
			float bottom;
			float top;
			juggler.getBoundingBox(left, right, bottom, top);

			context->translate(width / 2.0, height / 2.0);
			double scale = 0.9
				* std::min(width / (right - left), height / (top - bottom));
			context->scale(scale, -scale);
			context->translate(-(left + right) / 2.0, -(top + bottom) / 2.0);

			context->set_line_width(2.0 / scale);
		}

		CairoCanvas c(context);
		juggler.render(c, 0.1f);

		return true;
	}

	bool on_timeout()
	{
		Glib::RefPtr<Gdk::Window> window = get_window();
		if (!window)
			return true;

		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
			get_allocation().get_height());
		window->invalidate_rect(r, false);

		return true;
	}
};

int main(int argc, char** argv)
{
	if (argc != 2)
		// ???
		;

	SiteswapPattern pattern(argv[1]);
	Juggler juggler(pattern);

	Gtk::Main kit(argc, argv);

	Gtk::Window window;
	window.set_title("Juggler");

	MyDrawingArea d(juggler);
	window.add(d);
	d.show();

	Gtk::Main::run(window);

	return 0;
}
